#ifndef _SLATE_STRING_H
#define _SLATE_STRING_H

#include <features.h>
#include <stddef.h>

#define __NEED_size_t
#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_locale_t
#endif
#define __NEED_NULL
#include <bits/types.h>

void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
int   memcmp(const void *, const void *, size_t);
void *memchr(const void *, int, size_t);

char *strcpy(char *__restrict, const char *__restrict);
char *strncpy(char *__restrict, const char *__restrict, size_t);

char *strcat(char *__restrict, const char *__restrict);
char *strncat(char *__restrict, const char *__restrict, size_t);

int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);

int    strcoll(const char *, const char *);
size_t strxfrm(char *__restrict, const char *__restrict, size_t);

char *strchr(const char *, int);
char *strrchr(const char *, int);

size_t strcspn(const char *, const char *);
size_t strspn(const char *, const char *);
char  *strpbrk(const char *, const char *);
char  *strstr(const char *, const char *);
char  *strtok(char *__restrict, const char *__restrict);

size_t strlen(const char *);

char *strerror(int);

#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
#include <strings.h>
#endif

#if defined(_GNU_SOURCE)
char *strerror_r(int, char *, size_t);
#elif defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L ||                \
    defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600 ||                          \
    defined(_POSIX_SOURCE) || defined(_BSD_SOURCE)
int __xpg_strerror_r(int, char *, size_t);
#define strerror_r __xpg_strerror_r
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
char  *strtok_r(char *__restrict, const char *__restrict, char **__restrict);
char  *stpcpy(char *__restrict, const char *__restrict);
char  *stpncpy(char *__restrict, const char *__restrict, size_t);
size_t strnlen(const char *, size_t);
char  *strdup(const char *);
char  *strndup(const char *, size_t);
char  *strsignal(int);
char  *strerror_l(int, locale_t);
int    strcoll_l(const char *, const char *, locale_t);
size_t strxfrm_l(char *__restrict, const char *__restrict, size_t, locale_t);
void  *memmem(const void *, size_t, const void *, size_t);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void *memccpy(void *__restrict, const void *__restrict, int, size_t);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
char  *strsep(char **, const char *);
size_t strlcat(char *, const char *, size_t);
size_t strlcpy(char *, const char *, size_t);
void   explicit_bzero(void *, size_t);
#endif

#ifdef _GNU_SOURCE
char *strdupa(const char *x);
int   strverscmp(const char *, const char *);
char *strchrnul(const char *, int);
char *strcasestr(const char *, const char *);
void *memrchr(const void *, int, size_t);
void *mempcpy(void *, const void *, size_t);
#endif

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/locale/string.h>
#include <bits/msvc/wchar/string.h>
#endif

#endif
