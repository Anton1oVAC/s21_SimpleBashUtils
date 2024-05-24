#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define ALLOWED_FLAGS 'b', 'e', 'n', 's', 't', 'v', 'E', 'T'
#define FLAGS_COUNT 8

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int index;
} flags;

void parser_flags(int argc, char* argv[], flags* f);
void flag_valid(char* flag, flags* f);
void set_priority(int i, flags* f);
void print_file(int argc, char* argv[], flags* f);
void use_flags(unsigned char c, flags* f, int* line_number, int* nl_counter,
               int* empty_line, int* lineS, int* empty_s_line);

#endif