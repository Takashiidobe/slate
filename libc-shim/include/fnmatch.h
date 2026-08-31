#ifndef _SLATE_FNMATCH_H
#define _SLATE_FNMATCH_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/fnmatch.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/fnmatch.h>

#else

enum {
  FNM_PATHNAME    = 0x1,
  FNM_FILE_NAME   = FNM_PATHNAME,
  FNM_NOESCAPE    = 0x2,
  FNM_PERIOD      = 0x4,
  FNM_LEADING_DIR = 0x8,
  FNM_CASEFOLD    = 0x10,
  FNM_EXTMATCH    = 0x20,
};

enum {
  FNM_NOMATCH = 1,
  FNM_NOSYS   = -1,
};

int fnmatch(const char *, const char *, int);

#endif 

#endif
