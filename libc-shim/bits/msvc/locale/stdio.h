#ifndef _SLATE_BITS_MSVC_LOCALE_STDIO_H
#define _SLATE_BITS_MSVC_LOCALE_STDIO_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/locale/stdio.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

int _vfprintf_l(FILE *, const char *, _locale_t, va_list);
int _vfprintf_p_l(FILE *, const char *, _locale_t, va_list);
int _vprintf_l(const char *, _locale_t, va_list);
int _vprintf_p_l(const char *, _locale_t, va_list);
int _fprintf_l(FILE *, const char *, _locale_t, ...);
int _fprintf_p_l(FILE *, const char *, _locale_t, ...);
int _printf_l(const char *, _locale_t, ...);
int _printf_p_l(const char *, _locale_t, ...);

int _vfscanf_l(FILE *, const char *, _locale_t, va_list);
int _vscanf_l(const char *, _locale_t, va_list);
int _fscanf_l(FILE *, const char *, _locale_t, ...);
int _scanf_l(const char *, _locale_t, ...);

int _vscprintf_l(const char *, _locale_t, va_list);
int _vscprintf_p_l(const char *, _locale_t, va_list);
int _scprintf_l(const char *, _locale_t, ...);
int _scprintf_p_l(const char *, _locale_t, ...);
int _vsnprintf_l(char *, size_t, const char *, _locale_t, va_list);
int _vsnprintf_c_l(char *, size_t, const char *, _locale_t, va_list);
int _snprintf_l(char *, size_t, const char *, _locale_t, ...);
int _snprintf_c_l(char *, size_t, const char *, _locale_t, ...);
int _vsprintf_l(char *, const char *, _locale_t, va_list);
int _vsprintf_p_l(char *, size_t, const char *, _locale_t, va_list);
int _sprintf_l(char *, const char *, _locale_t, ...);
int _sprintf_p_l(char *, size_t, const char *, _locale_t, ...);
int _vsscanf_l(const char *, const char *, _locale_t, va_list);
int _sscanf_l(const char *, const char *, _locale_t, ...);
int _snscanf_l(const char *, size_t, const char *, _locale_t, ...);

#endif
