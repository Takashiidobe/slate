#ifndef _SLATE_BITS_MSVC_SECURE_STDLIB_H
#define _SLATE_BITS_MSVC_SECURE_STDLIB_H

#include <bits/msvc/types.h>
#include <bits/msvc/stddef.h>

typedef void (*_invalid_parameter_handler)(const wchar_t *, const wchar_t *,
                                           const wchar_t *, unsigned int,
                                           uintptr_t);

#if defined(_DEBUG)
void _invalid_parameter(const wchar_t *, const wchar_t *, const wchar_t *,
                        unsigned int, uintptr_t);
#endif
void _invalid_parameter_noinfo(void);
_Noreturn void _invalid_parameter_noinfo_noreturn(void);
_Noreturn void _invoke_watson(const wchar_t *, const wchar_t *,
                              const wchar_t *, unsigned int, uintptr_t);

_invalid_parameter_handler
_set_invalid_parameter_handler(_invalid_parameter_handler);
_invalid_parameter_handler _get_invalid_parameter_handler(void);
_invalid_parameter_handler
_set_thread_local_invalid_parameter_handler(_invalid_parameter_handler);
_invalid_parameter_handler _get_thread_local_invalid_parameter_handler(void);

#if __STDC_WANT_SECURE_LIB__
void *bsearch_s(const void *, const void *, rsize_t, rsize_t,
                int (*)(void *, const void *, const void *), void *);
void  qsort_s(void *, rsize_t, rsize_t,
              int (*)(void *, const void *, const void *), void *);
errno_t wctomb_s(int *, char *, rsize_t, wchar_t);
errno_t getenv_s(size_t *, char *, rsize_t, const char *);
#endif

errno_t _itoa_s(int, char *, size_t, int);
errno_t _ltoa_s(long, char *, size_t, int);
errno_t _ultoa_s(unsigned long, char *, size_t, int);
errno_t _i64toa_s(long long, char *, size_t, int);
errno_t _ui64toa_s(unsigned long long, char *, size_t, int);
errno_t _ecvt_s(char *, size_t, double, int, int *, int *);
errno_t _fcvt_s(char *, size_t, double, int, int *, int *);
errno_t _gcvt_s(char *, size_t, double, int);

errno_t mbstowcs_s(size_t *, wchar_t *, size_t, const char *, size_t);
errno_t _mbstowcs_s_l(size_t *, wchar_t *, size_t, const char *, size_t,
                      _locale_t);
errno_t _wctomb_s_l(int *, char *, size_t, wchar_t, _locale_t);
errno_t wcstombs_s(size_t *, char *, size_t, const wchar_t *, size_t);
errno_t _wcstombs_s_l(size_t *, char *, size_t, const wchar_t *, size_t,
                      _locale_t);

errno_t _makepath_s(char *, size_t, const char *, const char *, const char *,
                    const char *);
errno_t _splitpath_s(const char *, char *, size_t, char *, size_t, char *,
                     size_t, char *, size_t);
errno_t _dupenv_s(char **, size_t *, const char *);
errno_t _putenv_s(const char *, const char *);
errno_t _searchenv_s(const char *, const char *, char *, size_t);

errno_t _itow_s(int, wchar_t *, size_t, int);
errno_t _ltow_s(long, wchar_t *, size_t, int);
errno_t _ultow_s(unsigned long, wchar_t *, size_t, int);
errno_t _i64tow_s(long long, wchar_t *, size_t, int);
errno_t _ui64tow_s(unsigned long long, wchar_t *, size_t, int);
errno_t _wmakepath_s(wchar_t *, size_t, const wchar_t *, const wchar_t *,
                     const wchar_t *, const wchar_t *);
errno_t _wsplitpath_s(const wchar_t *, wchar_t *, size_t, wchar_t *, size_t,
                      wchar_t *, size_t, wchar_t *, size_t);
errno_t _wdupenv_s(wchar_t **, size_t *, const wchar_t *);
errno_t _wgetenv_s(size_t *, wchar_t *, size_t, const wchar_t *);
errno_t _wputenv_s(const wchar_t *, const wchar_t *);
errno_t _wsearchenv_s(const wchar_t *, const wchar_t *, wchar_t *, size_t);

#endif
