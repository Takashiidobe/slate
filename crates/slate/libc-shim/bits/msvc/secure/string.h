#ifndef _SLATE_BITS_MSVC_SECURE_STRING_H
#define _SLATE_BITS_MSVC_SECURE_STRING_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/secure/string.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>
#include <bits/msvc/stddef.h>

#if __STDC_WANT_SECURE_LIB__
errno_t memcpy_s(void *, rsize_t, const void *, rsize_t);
errno_t memmove_s(void *, rsize_t, const void *, rsize_t);
errno_t strcpy_s(char *, rsize_t, const char *);
errno_t strcat_s(char *, rsize_t, const char *);
errno_t strerror_s(char *, size_t, int);
errno_t strncat_s(char *, rsize_t, const char *, rsize_t);
errno_t strncpy_s(char *, rsize_t, const char *, rsize_t);
char   *strtok_s(char *, const char *, char **);
size_t  strnlen_s(const char *, size_t);
#endif

errno_t _strerror_s(char *, size_t, const char *);
errno_t _strlwr_s(char *, size_t);
errno_t _strlwr_s_l(char *, size_t, _locale_t);
errno_t _strnset_s(char *, size_t, int, size_t);
errno_t _strset_s(char *, size_t, int);
errno_t _strupr_s(char *, size_t);
errno_t _strupr_s_l(char *, size_t, _locale_t);

#endif
