#ifndef _SLATE_GETOPT_H
#define _SLATE_GETOPT_H

int          getopt(int, char *const[], const char *);
extern char *optarg;
extern int   optind, opterr, optopt, optreset;

struct option {
  const char *name;
  int         has_arg;
  int        *flag;
  int         val;
};

int getopt_long(int, char *const *, const char *, const struct option *, int *);
int getopt_long_only(int, char *const *, const char *, const struct option *,
                     int *);

enum {
  no_argument       = 0,
  required_argument = 1,
  optional_argument = 2,
};

#endif
