#ifndef _SLATE_BITS_MSVC_SECURE_WCHAR_H
#define _SLATE_BITS_MSVC_SECURE_WCHAR_H

#include <bits/msvc/types.h>
#include <bits/msvc/stddef.h>

errno_t mbsrtowcs_s(size_t *, wchar_t *, size_t, const char **, size_t,
                    mbstate_t *);
errno_t wcrtomb_s(size_t *, char *, size_t, wchar_t, mbstate_t *);
errno_t wcsrtombs_s(size_t *, char *, size_t, const wchar_t **, size_t,
                    mbstate_t *);

#if __STDC_WANT_SECURE_LIB__
errno_t wcscat_s(wchar_t *, rsize_t, const wchar_t *);
errno_t wcscpy_s(wchar_t *, rsize_t, const wchar_t *);
errno_t wcsncat_s(wchar_t *, rsize_t, const wchar_t *, rsize_t);
errno_t wcsncpy_s(wchar_t *, rsize_t, const wchar_t *, rsize_t);
wchar_t *wcstok_s(wchar_t *, const wchar_t *, wchar_t **);
size_t   wcsnlen_s(const wchar_t *, size_t);
errno_t wmemcpy_s(wchar_t *, rsize_t, const wchar_t *, rsize_t);
errno_t wmemmove_s(wchar_t *, rsize_t, const wchar_t *, rsize_t);

int vfwprintf_s(FILE *, const wchar_t *, va_list);
int vwprintf_s(const wchar_t *, va_list);
int fwprintf_s(FILE *, const wchar_t *, ...);
int wprintf_s(const wchar_t *, ...);
int vfwscanf_s(FILE *, const wchar_t *, va_list);
int vwscanf_s(const wchar_t *, va_list);
int fwscanf_s(FILE *, const wchar_t *, ...);
int wscanf_s(const wchar_t *, ...);
int vswprintf_s(wchar_t *, size_t, const wchar_t *, va_list);
int swprintf_s(wchar_t *, size_t, const wchar_t *, ...);
int vswscanf_s(const wchar_t *, const wchar_t *, va_list);
int swscanf_s(const wchar_t *, const wchar_t *, ...);
#endif

errno_t _wcserror_s(wchar_t *, size_t, int);
errno_t __wcserror_s(wchar_t *, size_t, const wchar_t *);
errno_t _wcsnset_s(wchar_t *, size_t, wchar_t, size_t);
errno_t _wcsset_s(wchar_t *, size_t, wchar_t);
errno_t _wcslwr_s(wchar_t *, size_t);
errno_t _wcslwr_s_l(wchar_t *, size_t, _locale_t);
errno_t _wcsupr_s(wchar_t *, size_t);
errno_t _wcsupr_s_l(wchar_t *, size_t, _locale_t);

wchar_t *_getws_s(wchar_t *, size_t);
errno_t  _wfopen_s(FILE **, const wchar_t *, const wchar_t *);
errno_t  _wfreopen_s(FILE **, const wchar_t *, const wchar_t *, FILE *);
errno_t  _wtmpnam_s(wchar_t *, size_t);

int _vfwprintf_s_l(FILE *, const wchar_t *, _locale_t, va_list);
int _vwprintf_s_l(const wchar_t *, _locale_t, va_list);
int _fwprintf_s_l(FILE *, const wchar_t *, _locale_t, ...);
int _wprintf_s_l(const wchar_t *, _locale_t, ...);
int _vfwscanf_s_l(FILE *, const wchar_t *, _locale_t, va_list);
int _vwscanf_s_l(const wchar_t *, _locale_t, va_list);
int _fwscanf_s_l(FILE *, const wchar_t *, _locale_t, ...);
int _wscanf_s_l(const wchar_t *, _locale_t, ...);
int _vsnwprintf_s(wchar_t *, size_t, size_t, const wchar_t *, va_list);
int _vsnwprintf_s_l(wchar_t *, size_t, size_t, const wchar_t *, _locale_t,
                    va_list);
int _snwprintf_s(wchar_t *, size_t, size_t, const wchar_t *, ...);
int _snwprintf_s_l(wchar_t *, size_t, size_t, const wchar_t *, _locale_t, ...);
int _vswprintf_s_l(wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int _swprintf_s_l(wchar_t *, size_t, const wchar_t *, _locale_t, ...);
int _vswscanf_s_l(const wchar_t *, const wchar_t *, _locale_t, va_list);
int _swscanf_s_l(const wchar_t *, const wchar_t *, _locale_t, ...);
int _snwscanf_s(const wchar_t *, size_t, const wchar_t *, ...);
int _snwscanf_s_l(const wchar_t *, size_t, const wchar_t *, _locale_t, ...);

errno_t _wasctime_s(wchar_t *, size_t, const struct tm *);
errno_t _wctime32_s(wchar_t *, size_t, const __time32_t *);
errno_t _wctime64_s(wchar_t *, size_t, const __time64_t *);
errno_t _wstrdate_s(wchar_t *, size_t);
errno_t _wstrtime_s(wchar_t *, size_t);

#if __STDC_WANT_SECURE_LIB__
errno_t _wctime_s(wchar_t *, size_t, const time_t *);
#endif

#endif
