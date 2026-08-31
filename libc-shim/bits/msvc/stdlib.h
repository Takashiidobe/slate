#ifndef _SLATE_BITS_MSVC_STDLIB_H
#define _SLATE_BITS_MSVC_STDLIB_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/stdlib.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

#define _MAX_ENV 32767

char    **__p__pgmptr(void);
wchar_t **__p__wpgmptr(void);
errno_t   _get_pgmptr(char **);
errno_t   _get_wpgmptr(wchar_t **);

int       *__p___argc(void);
char    ***__p___argv(void);
wchar_t ***__p___wargv(void);
char    ***__p__environ(void);
wchar_t ***__p__wenviron(void);

int      _initialize_narrow_environment(void);
int      _initialize_wide_environment(void);
char   **_get_initial_narrow_environment(void);
wchar_t **_get_initial_wide_environment(void);

wchar_t *_wgetenv(const wchar_t *);
int      _putenv(const char *);
int      _wputenv(const wchar_t *);
void     _searchenv(const char *, const char *, char *);
void     _wsearchenv(const wchar_t *, const wchar_t *, wchar_t *);
int      _wsystem(const wchar_t *);

#include <bits/msvc/locale/stdlib.h>
#include <bits/msvc/secure/stdlib.h>
#include <bits/msvc/wchar/stdlib.h>

#endif
