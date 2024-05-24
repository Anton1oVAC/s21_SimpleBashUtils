#include <stdio.h>

#include "s21_cat.h"

int main(int argc, char* argv[]) {
  flags f = {0};
  parser_flags(argc, argv, &f);
  print_file(argc, argv, &f);
  return 0;
}

void parser_flags(int argc, char* argv[], flags* f) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      flag_valid(argv[i] + 1, f);
    } else if (f->index == 0) {
      f->index = i;
    }
  }
}

void flag_valid(char* flag, flags* f) {
  char allowed_flags[FLAGS_COUNT] = {ALLOWED_FLAGS};
  for (; *flag; flag++) {
    for (int i = 0; i < FLAGS_COUNT; i++) {
      if (*flag == allowed_flags[i]) {
        set_priority(i, f);
      }
    }
  }
}

void set_priority(int i, flags* f) {
  switch (i) {
    case 0:
      f->b = 1;
      f->n = 0;
      break;
    case 1:
      f->e = 1;
      f->v = 1;
      break;
    case 2:
      if (f->b == 0) f->n = 1;
      break;
    case 3:
      f->s = 1;
      break;
    case 4:
      f->t = 1;
      f->v = 1;
      break;
    case 5:
      f->v = 1;
      break;
    case 6:
      f->e = 1;
      break;
    case 7:
      f->t = 1;
      break;
  }
}

void print_file(int argc, char* argv[], flags* f) {
  for (int i = f->index; i < argc; i++) {
    FILE* file = fopen(argv[i], "r");
    if (file != NULL) {
      char c;
      int lineS = 0;
      int empty_s_line = 1;
      int line_number = 1;
      int nl_counter = 1;
      int empty_line = 1;
      while ((c = fgetc(file)) != EOF) {
        use_flags(c, f, &line_number, &nl_counter, &empty_line, &lineS,
                  &empty_s_line);
      }
      fclose(file);
    }
    if (file == NULL) {
      fprintf(stderr, "cat: %s : No such file or directory\n", argv[i]);
    }
  }
}

void use_flags(unsigned char c, flags* f, int* line_number, int* nl_counter,
               int* empty_line, int* lineS, int* empty_s_line) {
  if (f->s) {
    if (c == '\n' && *empty_s_line == 1) {
      (*lineS)++;
    } else if (c == '\n' && *empty_s_line == 0) {
      *lineS = 0;
      *empty_s_line = 1;
    } else {
      *empty_s_line = 0;
      *lineS = 0;
    }
  }

  if (f->b && *lineS < 2) {
    if (c != '\n' && *empty_line == 1) {
      printf("%6d\t", *nl_counter);
      *empty_line = 0;
    } else if (c == '\n' && *empty_line == 0) {
      *empty_line = 1;
      (*nl_counter)++;
    }
  }

  if (f->n && *lineS < 2) {
    if (c == '\n' && *empty_line == 1) {
      printf("%6d\t", *nl_counter);
      (*nl_counter)++;
    } else if (*empty_line == 1) {
      printf("%6d\t", *nl_counter);
      *empty_line = 0;
    } else if (c == '\n' && *empty_line == 0) {
      *empty_line = 1;
      (*nl_counter)++;
    }
  }

  if (*line_number < 3 && *lineS < 2) {
    if (c == '\n' && f->e) {
      printf("$");
    }
    if (c == '\t' && f->t) {
      printf("^");
      (c = 'I');
    }
    if (f->v) {
      if (c > 127 && c < 160) printf("M-^");
      if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
      if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
        c = c > 126 ? c - 128 + 64 : c + 64;
    }
  }
  if (*lineS < 2) putchar(c);
}
