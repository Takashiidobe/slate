#ifndef _SLATE_FNMATCH_H
#define _SLATE_FNMATCH_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/fnmatch.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/fnmatch.h>

#else

#define FNM_PATHNAME    0x1
#define FNM_FILE_NAME   FNM_PATHNAME
#define FNM_NOESCAPE    0x2
#define FNM_PERIOD      0x4
#define FNM_LEADING_DIR 0x8
#define FNM_CASEFOLD    0x10
#if defined(__SLATE_LIBC_GLIBC)
#define FNM_EXTMATCH 0x20
#endif

#define FNM_NOMATCH 1
#define FNM_NOSYS   (-1)

int fnmatch(const char *, const char *, int);

#endif 

#endif
