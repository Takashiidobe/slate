#ifndef _SLATE_STDIO_EXT_H
#define _SLATE_STDIO_EXT_H

#include <stdio.h>

enum {
  FSETLOCKING_QUERY    = 0,
  FSETLOCKING_INTERNAL = 1,
  FSETLOCKING_BYCALLER = 2,
};

void   _flushlbf(void);
int    __fsetlocking(FILE *, int);
int    __fwriting(FILE *);
int    __freading(FILE *);
int    __freadable(FILE *);
int    __fwritable(FILE *);
int    __flbf(FILE *);
size_t __fbufsize(FILE *);
size_t __fpending(FILE *);
int    __fpurge(FILE *);

size_t      __freadahead(FILE *);
const char *__freadptr(FILE *, size_t *);
void        __freadptrinc(FILE *, size_t);
void        __fseterr(FILE *);

#endif
