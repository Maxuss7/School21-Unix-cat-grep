#include "s21_grep.h"

#include <stdlib.h>
#include <string.h>

void init_ctx(Context *ctx) {
  memset(ctx, 0, sizeof(*ctx));
  ctx->h = 0;
}

int parse_args(int argc, char **argv, Context *ctx) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      if (ctx->pat_count == 0) {
        strncpy(ctx->patterns[ctx->pat_count++], argv[i], MAX_LINE);
      } else {
        ctx->files = &argv[i];
        ctx->file_count = argc - i;
        break;
      }
      continue;
    }

    for (char *p = &argv[i][1]; *p; p++) {
      if (handle_option(*p, ctx, &i, argv) != 0) {
        return 1;
      }
    }
  }
  return !ctx->pat_count || !ctx->file_count;
}

int handle_option(char opt, Context *ctx, int *arg_idx, char **argv) {
  switch (opt) {
    case 'e':
      return handle_pattern_option(ctx, arg_idx, argv);
    case 'f':
      return handle_file_option(ctx, arg_idx, argv);
    case 'i':
      ctx->i = 1;
      break;
    case 'v':
      ctx->v = 1;
      break;
    case 'c':
      ctx->c = 1;
      break;
    case 'l':
      ctx->l = 1;
      break;
    case 'n':
      ctx->n = 1;
      break;
    case 'h':
      ctx->h = 1;
      break;
    case 's':
      ctx->s = 1;
      break;
    case 'o':
      ctx->o = 1;
      break;
    default:
      fprintf(stderr, "grep: invalid option -- '%c'\n", opt);
      return 1;
  }
  return 0;
}

int handle_pattern_option(Context *ctx, int *arg_idx, char **argv) {
  if (argv[*arg_idx + 1] == NULL) {
    fprintf(stderr, "grep: option requires an argument -- 'e'\n");
    return 1;
  }
  strncpy(ctx->patterns[ctx->pat_count++], argv[++(*arg_idx)], MAX_LINE);
  return 0;
}

int handle_file_option(Context *ctx, int *arg_idx, char **argv) {
  if (argv[*arg_idx + 1] == NULL) {
    fprintf(stderr, "grep: option requires an argument -- 'f'\n");
    return 1;
  }
  load_patterns(ctx, argv[++(*arg_idx)]);
  return 0;
}

void load_patterns(Context *ctx, const char *fn) {
  FILE *fp = fopen(fn, "r");
  if (!fp) {
    if (!ctx->s) fprintf(stderr, "grep: %s: No such file or directory\n", fn);
    return;
  }

  char line[MAX_LINE];
  while (fgets(line, sizeof(line), fp) && ctx->pat_count < MAX_PAT) {
    line[strcspn(line, "\n")] = '\0';
    strncpy(ctx->patterns[ctx->pat_count++], line, MAX_LINE);
  }
  fclose(fp);
}

void normalize_line(char *line) {
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = '\0';
  }
}

int check_line_match(const char *line, regex_t *re, int cnt) {
  for (int i = 0; i < cnt; i++) {
    if (regexec(&re[i], line, 0, NULL, 0) == 0) {
      return 1;
    }
  }
  return 0;
}

void process_file(Context *ctx, const char *fn) {
  FILE *fp = open_input_file(fn, ctx);
  if (!fp) return;

  regex_t re[MAX_PAT];
  int compiled = compile_patterns(ctx, re);
  char line[MAX_LINE];
  int lnum = 0, matches = 0;

  while (fgets(line, sizeof(line), fp)) {
    lnum++;
    normalize_line(line);

    int match = check_line_match(line, re, compiled);
    if ((match && !ctx->v) || (!match && ctx->v)) {
      matches++;

      if (ctx->l) {
        printf("%s\n", fn);
        break;
      }

      if (!ctx->c && !ctx->o) {
        print_matching_line(line, lnum, ctx, fn);
      }

      if (ctx->o && match) {
        extract_matches(line, re, compiled, ctx, fn, lnum);
      }
    }
  }

  if (ctx->c && !ctx->l) {
    print_match_count(ctx, fn, matches);
  }

  cleanup_resources(fp, re, compiled);
}

int compile_patterns(Context *ctx, regex_t *re) {
  int compiled;
  for (compiled = 0; compiled < ctx->pat_count; compiled++) {
    if (regcomp(&re[compiled], ctx->patterns[compiled],
                REG_EXTENDED | (ctx->i ? REG_ICASE : 0))) {
      break;
    }
  }
  return compiled;
}

void print_matching_line(const char *line, int lnum, Context *ctx,
                         const char *fn) {
  const int print_name = !ctx->h && ctx->file_count > 1;
  if (print_name) printf("%s:", fn);
  if (ctx->n) printf("%d:", lnum);
  printf("%s\n", line);
}

void print_match_count(Context *ctx, const char *fn, int count) {
  const int print_name = !ctx->h && ctx->file_count > 1;
  if (print_name) printf("%s:", fn);
  printf("%d\n", count);
}

void extract_matches(const char *line, regex_t *re, int cnt, Context *ctx,
                     const char *fn, int line_num) {
  regmatch_t match;
  const int print_name = !ctx->h && ctx->file_count > 1;

  for (int i = 0; i < cnt; i++) {
    const char *ptr = line;
    while (regexec(&re[i], ptr, 1, &match, 0) == 0) {
      if (match.rm_so == -1) break;

      // Выводим имя файла если нужно
      if (print_name) printf("%s:", fn);

      // Выводим номер строки если флаг -n установлен
      if (ctx->n) printf("%d:", line_num);

      // Выводим само совпадение
      printf("%.*s\n", (int)(match.rm_eo - match.rm_so), ptr + match.rm_so);

      ptr += match.rm_eo;
    }
  }
}

FILE *open_input_file(const char *fn, Context *ctx) {
  FILE *fp = strcmp(fn, "-") == 0 ? stdin : fopen(fn, "r");
  if (!fp && !ctx->s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", fn);
  }
  return fp;
}

void cleanup_resources(FILE *fp, regex_t *re, int count) {
  for (int i = 0; i < count; i++) {
    regfree(&re[i]);
  }
  if (fp != stdin) {
    fclose(fp);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
    return 2;
  }

  Context ctx;
  init_ctx(&ctx);

  if (parse_args(argc, argv, &ctx)) {
    fprintf(stderr, "Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
    return 2;
  }

  for (int i = 0; i < ctx.file_count; i++) {
    process_file(&ctx, ctx.files[i]);
  }

  return 0;
}
