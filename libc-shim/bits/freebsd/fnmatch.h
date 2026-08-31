#ifndef _SLATE_BITS_FREEBSD_FNMATCH_H
#define _SLATE_BITS_FREEBSD_FNMATCH_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/fnmatch.h> directly; include a public header instead."
#endif

#define FNM_NOMATCH 1

#define FNM_NOESCAPE 0x01
#define FNM_PATHNAME 0x02
#define FNM_PERIOD   0x04

#define FNM_NOSYS (-1)

#define FNM_LEADING_DIR 0x08
#define FNM_CASEFOLD    0x10
#define FNM_IGNORECASE  FNM_CASEFOLD
#define FNM_FILE_NAME   FNM_PATHNAME

int fnmatch(const char *, const char *, int);

#endif
