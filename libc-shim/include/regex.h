#ifndef _REGEX_H
#define _REGEX_H

#include <features.h>

#define __NEED_regoff_t
#define __NEED_size_t
#include <bits/types.h>

typedef struct re_pattern_buffer {
  size_t re_nsub;
  void  *__opaque, *__padding[4];
  size_t __nsub2;
  char   __padding2;
} regex_t;

typedef struct {
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;

enum {
  REG_EXTENDED = 1,
  REG_ICASE    = 2,
  REG_NEWLINE  = 4,
  REG_NOSUB    = 8,
};

enum {
  REG_NOTBOL = 1,
  REG_NOTEOL = 2,
};

enum {
  REG_OK       = 0,
  REG_NOMATCH  = 1,
  REG_BADPAT   = 2,
  REG_ECOLLATE = 3,
  REG_ECTYPE   = 4,
  REG_EESCAPE  = 5,
  REG_ESUBREG  = 6,
  REG_EBRACK   = 7,
  REG_EPAREN   = 8,
  REG_EBRACE   = 9,
  REG_BADBR    = 10,
  REG_ERANGE   = 11,
  REG_ESPACE   = 12,
  REG_BADRPT   = 13,
  REG_ENOSYS   = -1,
};

int  regcomp(regex_t *__restrict, const char *__restrict, int);
int  regexec(const regex_t *__restrict, const char *__restrict, size_t,
             regmatch_t *__restrict, int);
void regfree(regex_t *);

size_t regerror(int, const regex_t *__restrict, char *__restrict, size_t);

#endif
