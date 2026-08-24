#ifndef _SLATE_BITS_MSVC_SECURE_STDIO_H
#define _SLATE_BITS_MSVC_SECURE_STDIO_H

#include <bits/msvc/types.h>
#include <bits/msvc/stddef.h>

#if __STDC_WANT_SECURE_LIB__
errno_t clearerr_s(FILE *);
errno_t fopen_s(FILE **, const char *, const char *);
size_t  fread_s(void *, size_t, size_t, size_t, FILE *);
errno_t freopen_s(FILE **, const char *, const char *, FILE *);
char   *gets_s(char *, rsize_t);
errno_t tmpfile_s(FILE **);
errno_t tmpnam_s(char *, rsize_t);

int vfprintf_s(FILE *, const char *, va_list);
int vprintf_s(const char *, va_list);
int fprintf_s(FILE *, const char *, ...);
int printf_s(const char *, ...);
int vfscanf_s(FILE *, const char *, va_list);
int vscanf_s(const char *, va_list);
int fscanf_s(FILE *, const char *, ...);
int scanf_s(const char *, ...);
int vsprintf_s(char *, size_t, const char *, va_list);
int vsnprintf_s(char *, size_t, size_t, const char *, va_list);
int sprintf_s(char *, size_t, const char *, ...);
int vsscanf_s(const char *, const char *, va_list);
int sscanf_s(const char *, const char *, ...);
#endif

size_t _fread_nolock_s(void *, size_t, size_t, size_t, FILE *);

int _vfprintf_s_l(FILE *, const char *, _locale_t, va_list);
int _vprintf_s_l(const char *, _locale_t, va_list);
int _fprintf_s_l(FILE *, const char *, _locale_t, ...);
int _printf_s_l(const char *, _locale_t, ...);
int _vfscanf_s_l(FILE *, const char *, _locale_t, va_list);
int _vscanf_s_l(const char *, _locale_t, va_list);
int _fscanf_s_l(FILE *, const char *, _locale_t, ...);
int _scanf_s_l(const char *, _locale_t, ...);
int _vsnprintf_s(char *, size_t, size_t, const char *, va_list);
int _vsnprintf_s_l(char *, size_t, size_t, const char *, _locale_t, va_list);
int _snprintf_s(char *, size_t, size_t, const char *, ...);
int _snprintf_s_l(char *, size_t, size_t, const char *, _locale_t, ...);
int _vsprintf_s_l(char *, size_t, const char *, _locale_t, va_list);
int _sprintf_s_l(char *, size_t, const char *, _locale_t, ...);
int _vsscanf_s_l(const char *, const char *, _locale_t, va_list);
int _sscanf_s_l(const char *, const char *, _locale_t, ...);
int _snscanf_s(const char *, size_t, const char *, ...);
int _snscanf_s_l(const char *, size_t, const char *, _locale_t, ...);

#endif
