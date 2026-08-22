#ifndef _SLATE_ARGP_H
#define _SLATE_ARGP_H

#include <errno.h>
#include <features.h>
#include <stdio.h>

#ifndef __error_t_defined
#define __error_t_defined 1
typedef int error_t;
#endif

struct argp_option {
  const char *name;
  int         key;
  const char *arg;
  int         flags;
  const char *doc;
  int         group;
};

#define OPTION_ARG_OPTIONAL 0x1
#define OPTION_HIDDEN 0x2
#define OPTION_ALIAS 0x4
#define OPTION_DOC 0x8
#define OPTION_NO_USAGE 0x10

struct argp;
struct argp_state;
struct argp_child;

typedef error_t (*argp_parser_t)(int key, char *arg, struct argp_state *state);

#define ARGP_ERR_UNKNOWN E2BIG

#define ARGP_KEY_ARG 0
#define ARGP_KEY_ARGS 0x1000006
#define ARGP_KEY_END 0x1000001
#define ARGP_KEY_NO_ARGS 0x1000002
#define ARGP_KEY_INIT 0x1000003
#define ARGP_KEY_FINI 0x1000007
#define ARGP_KEY_SUCCESS 0x1000004
#define ARGP_KEY_ERROR 0x1000005

struct argp {
  const struct argp_option *options;
  argp_parser_t             parser;
  const char               *args_doc;
  const char               *doc;
  const struct argp_child  *children;
  char *(*help_filter)(int key, const char *text, void *input);
  const char *argp_domain;
};

#define ARGP_KEY_HELP_PRE_DOC 0x2000001
#define ARGP_KEY_HELP_POST_DOC 0x2000002
#define ARGP_KEY_HELP_HEADER 0x2000003
#define ARGP_KEY_HELP_EXTRA 0x2000004
#define ARGP_KEY_HELP_DUP_ARGS_NOTE 0x2000005
#define ARGP_KEY_HELP_ARGS_DOC 0x2000006

struct argp_child {
  const struct argp *argp;
  int                 flags;
  const char         *header;
  int                 group;
};

struct argp_state {
  const struct argp *root_argp;
  int                 argc;
  char              **argv;
  int                 next;
  unsigned            flags;
  unsigned            arg_num;
  int                 quoted;
  void               *input;
  void              **child_inputs;
  void               *hook;
  char               *name;
  FILE               *err_stream;
  FILE               *out_stream;
  void               *pstate;
};

#define ARGP_PARSE_ARGV0 0x01
#define ARGP_NO_ERRS 0x02
#define ARGP_NO_ARGS 0x04
#define ARGP_IN_ORDER 0x08
#define ARGP_NO_HELP 0x10
#define ARGP_NO_EXIT 0x20
#define ARGP_LONG_ONLY 0x40
#define ARGP_SILENT (ARGP_NO_EXIT | ARGP_NO_ERRS | ARGP_NO_HELP)

error_t argp_parse(const struct argp *__restrict, int, char **__restrict,
                   unsigned, int *__restrict, void *__restrict);

void argp_help(const struct argp *__restrict, FILE *__restrict, unsigned,
              char *__restrict);
void argp_state_help(const struct argp_state *__restrict, FILE *__restrict,
                     unsigned);
void argp_usage(const struct argp_state *);
void argp_error(const struct argp_state *__restrict, const char *__restrict,
                ...);
void argp_failure(const struct argp_state *__restrict, int, int,
                  const char *__restrict, ...);

extern const char *argp_program_version;
extern const char *argp_program_bug_address;
extern void (*argp_program_version_hook)(FILE *, struct argp_state *);

#endif
