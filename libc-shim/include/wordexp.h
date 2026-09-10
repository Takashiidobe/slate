#ifndef _SLATE_WORDEXP_H
#define _SLATE_WORDEXP_H

#include <features.h>

#define __NEED_size_t
#if defined(__SLATE_LIBC_MUSL)
#define WRDE_DOOFFS 1
#define WRDE_APPEND 2
#define WRDE_NOCMD 4
#define WRDE_REUSE 8
#define WRDE_SHOWERR 16
#define WRDE_UNDEF 32
#define WRDE_NOSYS -1
#define WRDE_NOSPACE 1
#define WRDE_BADCHAR 2
#define WRDE_BADVAL 3
#define WRDE_CMDSUB 4
#define WRDE_SYNTAX 5
#else
enum {
  WRDE_DOOFFS  = (1 << 0),
  WRDE_APPEND  = (1 << 1),
  WRDE_NOCMD   = (1 << 2),
  WRDE_REUSE   = (1 << 3),
  WRDE_SHOWERR = (1 << 4),
  WRDE_UNDEF   = (1 << 5),
};
#endif
#include <bits/types.h>

typedef struct {
  size_t we_wordc;
  char **we_wordv;
  size_t we_offs;
} wordexp_t;

#if !defined(__SLATE_LIBC_MUSL)
enum {
  WRDE_NOSYS   = -1,
  WRDE_NOSPACE = 1,
  WRDE_BADCHAR = 2,
  WRDE_BADVAL  = 3,
  WRDE_CMDSUB  = 4,
  WRDE_SYNTAX  = 5,
};
#endif

int  wordexp(const char *, wordexp_t *, int);
void wordfree(wordexp_t *);

#endif
