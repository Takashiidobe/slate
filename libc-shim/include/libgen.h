#ifndef _SLATE_LIBGEN_H
#define _SLATE_LIBGEN_H

#include <features.h>

char *dirname(char *);
#if defined(__SLATE_LIBC_GLIBC)
char *__xpg_basename(char *);
#define basename __xpg_basename
#else
char *basename(char *);
#endif

#endif
