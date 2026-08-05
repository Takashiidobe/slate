#ifndef _SLATE_WORDEXP_H
#define _SLATE_WORDEXP_H

#define __NEED_size_t

typedef unsigned long size_t;

enum {
  WRDE_DOOFFS = (1 << 0),
  WRDE_APPEND = (1 << 1),
  WRDE_NOCMD = (1 << 2),
  WRDE_REUSE = (1 << 3),
  WRDE_SHOWERR = (1 << 4),
  WRDE_UNDEF = (1 << 5),
};

typedef struct {
  size_t we_wordc;
  char **we_wordv;
  size_t we_offs;
} wordexp_t;

enum {
  WRDE_NOSYS = -1,
  WRDE_NOSPACE = 1,
  WRDE_BADCHAR = 2,
  WRDE_BADVAL = 3,
  WRDE_CMDSUB = 4,
  WRDE_SYNTAX = 5,
};

int wordexp(const char *, wordexp_t *, int);
void wordfree(wordexp_t *);

#endif
