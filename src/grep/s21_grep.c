#include "s21_grep.h"

int main(int argc, char* argv[]) {
  Flags flags = {0};
  if (argc > 2) {
    valid_grep(argc, argv, &flags);
    if (flags.e == 0 && flags.f == 0) {
      strcat(flags.pattern, argv[optind]);
      optind += 1;
    }
    open_files(argc, argv, &flags);
  }
  return 0;
}

void valid_grep(int argc, char* argv[], Flags* flags) {
  int i = 0;
  int counter_e = 0;
  flags->pattern_count = 0;
  while ((i = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (i) {
      case 'e':
        flags->e = 1;
        if (counter_e == 1) {
          strcat(flags->pattern, "|");
        }
        strcat(flags->pattern, optarg);
        flags->pattern_count += strlen(optarg);
        flags->pattern_count++;
        counter_e = 1;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        grep_flag_f(optarg, &counter_e, flags, argv);
        break;
      case 'o':
        flags->o = 1;
        break;
    }
  }
}

void grep_flag_f(char* optarg, int* counter_e, Flags* flags, char* argv[]) {
  FILE* fp = NULL;
  char line[10000] = {0};
  int counter_f = 0;
  if ((fp = fopen(optarg, "r")) == NULL) {
    if (!flags->s)
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], optarg);
  } else {
    while (fgets(line, 10000, fp) != NULL) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
      if (*counter_e > 0) strcat(flags->pattern, "|");
      if (*counter_e == 0 && counter_f > 0) strcat(flags->pattern, "|");
      strcat(flags->pattern, line);
      counter_f++;
    }
    fclose(fp);
  }
}

void open_files(int argc, char* argv[], Flags* flags) {
  FILE* file = NULL;
  int files_count = argc - optind;
  for (; optind < argc; optind++) {
    if ((file = fopen(argv[optind], "r")) == NULL) {
      if (flags->s == 0) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
                argv[optind]);
      }
    } else {
      grep_flags(argv[optind], file, flags, files_count);
      fclose(file);
    }
  }
}

void grep_flags(char* argv, FILE* file, Flags* flags, int files_count) {
  regex_t regex;
  regmatch_t match = {0};
  size_t nmatch = 1;
  size_t n_line = 1;
  int err = 0;
  char str[10000];
  int lines_count = 0;
  int i_option = 0;
  if (flags->i == 1) i_option = REG_ICASE;
  err = regcomp(&regex, flags->pattern, REG_EXTENDED | i_option);
  while (!feof(file) && fgets(str, 10000, file)) {
    int new_line_o_counter = 1;
    err = regexec(&regex, str, nmatch, &match, 0);
    if (flags->v) err = !err;
    flags_cont(n_line, &lines_count, argv, err, flags, files_count,
               new_line_o_counter, &match, regex, str, nmatch);
    n_line++;
  }
  if (flags->c == 1) {
    if (files_count > 1 && flags->h == 0) {
      printf("%s:", argv);
    }
    if (flags->l == 1 && lines_count)
      printf("1\n");
    else
      printf("%d\n", lines_count);
  }
  if (flags->l == 1 && lines_count) {
    printf("%s\n", argv);
  }
  regfree(&regex);
}

void flags_cont(size_t n_line, int* lines_count, char* argv, int err,
                Flags* flags, int files_count, int new_line_o_counter,
                regmatch_t* match, regex_t regex, char* str, size_t nmatch) {
  if (err == 0) {
    if (flags->c == 0 && flags->l == 0) {
      if (files_count > 1 && flags->h == 0) {
        printf("%s:", argv);
      }
      if (flags->n != 0) {
        printf("%lu:", n_line);
      }
      if (flags->o != 0 && flags->v == 0) {
        new_line_o_counter = 0;
        flag_o(str, err, match, regex, nmatch);
      }
      if (flags->o == 0 || flags->v == 1) printf("%s", str);
      if (str[strlen(str) - 1] != '\n' && new_line_o_counter != 0) {
        printf("\n");
      }
    }
    (*lines_count)++;
  }
}

void flag_o(char* str, int err, regmatch_t* match, regex_t regex,
            size_t nmatch) {
  char* ptr = str;
  while (err == 0) {
    if (match->rm_eo == match->rm_so) break;
    printf("%.*s\n", (int)(match->rm_eo - match->rm_so), ptr + match->rm_so);
    ptr += match->rm_eo;
    err = regexec(&regex, ptr, nmatch, match, REG_NOTBOL);
  }
}