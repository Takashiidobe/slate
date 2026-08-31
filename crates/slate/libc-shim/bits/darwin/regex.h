#ifndef _SLATE_BITS_DARWIN_REGEX_H
#define _SLATE_BITS_DARWIN_REGEX_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/regex.h> directly; include a public header instead."
#endif

struct re_guts;

typedef struct {
  int             re_magic;
  size_t          re_nsub;
  const char     *re_endp;
  struct re_guts *re_g;
} regex_t;

typedef struct {
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;

#define REG_BASIC    0000
#define REG_EXTENDED 0001
#define REG_ICASE    0002
#define REG_NOSUB    0004
#define REG_NEWLINE  0010
#define REG_NOSPEC   0020
#define REG_PEND     0040
#define REG_DUMP     0200
#define REG_NOMATCH  1
#define REG_BADPAT   2
#define REG_ECOLLATE 3
#define REG_ECTYPE   4
#define REG_EESCAPE  5
#define REG_ESUBREG  6
#define REG_EBRACK   7
#define REG_EPAREN   8
#define REG_EBRACE   9
#define REG_BADBR    10
#define REG_ERANGE   11
#define REG_ESPACE   12
#define REG_BADRPT   13
#define REG_EMPTY    14
#define REG_ASSERT   15
#define REG_INVARG   16
#define REG_ATOI     255
#define REG_ITOA     0400
#define REG_NOTBOL   00001
#define REG_NOTEOL   00002
#define REG_STARTEND 00004

int  regcomp(regex_t *__restrict, const char *__restrict, int);
int  regexec(const regex_t *__restrict, const char *__restrict, size_t,
             regmatch_t *__restrict, int);
void regfree(regex_t *);
size_t regerror(int, const regex_t *__restrict, char *__restrict, size_t);

#endif
