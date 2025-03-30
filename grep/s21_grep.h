#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>
#include <stdio.h>

#define MAX_LINE 2048
#define MAX_PAT 100

typedef struct {
  int i, v, c, l, n, h, s, o;
  int pat_count;
  char patterns[MAX_PAT][MAX_LINE];
  int file_count;
  char **files;
} Context;

/* Main functions */
void init_ctx(Context *ctx);
int parse_args(int argc, char **argv, Context *ctx);
void load_patterns(Context *ctx, const char *fn);
void process_file(Context *ctx, const char *fn);
void extract_matches(const char *line, regex_t *re, int cnt, Context *ctx,
    const char *fn, int line_num);
void normalize_line(char *line);
int check_line_match(const char *line, regex_t *re, int cnt);
void print_match_count(Context *ctx, const char *fn, int count);

/* Helper functions */
int handle_option(char opt, Context *ctx, int *arg_idx, char **argv);
int handle_pattern_option(Context *ctx, int *arg_idx, char **argv);
int handle_file_option(Context *ctx, int *arg_idx, char **argv);
int compile_patterns(Context *ctx, regex_t *re);
void process_line(const char *line, int lnum, Context *ctx, regex_t *re,
                  const char *fn, int *matches);
void print_matching_line(const char *line, int lnum, Context *ctx,
                         const char *fn);
void cleanup_resources(FILE *fp, regex_t *re, int count);
FILE *open_input_file(const char *fn, Context *ctx);

#endif