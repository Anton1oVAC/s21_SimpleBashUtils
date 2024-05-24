#ifndef _SRC_GREP_S21_GREP_H_
#define _SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char pattern[10000];
  int pattern_count;
} Flags;

void valid_grep(int argc, char* argv[], Flags* flags);
void open_files(int argc, char* argv[], Flags* flags);
void grep_flag_f(char* optarg, int* counter_e, Flags* flags, char* argv[]);
void grep_flags(char* argv, FILE* fp, Flags* flags, int files_count);
void flags_cont(size_t n_line, int* lines_count, char* argv, int err,
                Flags* flags, int files_count, int new_line_o_counter,
                regmatch_t* match, regex_t regex, char* str, size_t nmatch);
void flag_o(char* str, int err, regmatch_t* match, regex_t regex,
            size_t nmatch);
#endif