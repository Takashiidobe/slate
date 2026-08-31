#ifndef _SLATE_BITS_MSVC_WCHAR_STDIO_H
#define _SLATE_BITS_MSVC_WCHAR_STDIO_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/wchar/stdio.h> directly; include a public header instead."
#endif

wint_t   fgetwc(FILE *);
wint_t   _fgetwchar(void);
wint_t   fputwc(wchar_t, FILE *);
wint_t   _fputwchar(wchar_t);
wint_t   getwc(FILE *);
wint_t   getwchar(void);
wchar_t *fgetws(wchar_t *, int, FILE *);
int      fputws(const wchar_t *, FILE *);
wint_t   putwc(wchar_t, FILE *);
wint_t   putwchar(wchar_t);
int      _putws(const wchar_t *);
wint_t   ungetwc(wint_t, FILE *);
FILE    *_wfdopen(int, const wchar_t *);
FILE    *_wfopen(const wchar_t *, const wchar_t *);
FILE    *_wfreopen(const wchar_t *, const wchar_t *, FILE *);
FILE    *_wfsopen(const wchar_t *, const wchar_t *, int);
void     _wperror(const wchar_t *);
FILE    *_wpopen(const wchar_t *, const wchar_t *);
int      _wremove(const wchar_t *);
wchar_t *_wtempnam(const wchar_t *, const wchar_t *);
wchar_t *_wtmpnam(wchar_t *);

wint_t _fgetwc_nolock(FILE *);
wint_t _fputwc_nolock(wchar_t, FILE *);
wint_t _getwc_nolock(FILE *);
wint_t _putwc_nolock(wchar_t, FILE *);
wint_t _ungetwc_nolock(wint_t, FILE *);

int vfwprintf(FILE *, const wchar_t *, va_list);
int vwprintf(const wchar_t *, va_list);
int _vfwprintf_l(FILE *, const wchar_t *, _locale_t, va_list);
int _vfwprintf_p(FILE *, const wchar_t *, va_list);
int _vfwprintf_p_l(FILE *, const wchar_t *, _locale_t, va_list);
int _vwprintf_l(const wchar_t *, _locale_t, va_list);
int _vwprintf_p(const wchar_t *, va_list);
int _vwprintf_p_l(const wchar_t *, _locale_t, va_list);
int fwprintf(FILE *, const wchar_t *, ...);
int wprintf(const wchar_t *, ...);
int _fwprintf_l(FILE *, const wchar_t *, _locale_t, ...);
int _fwprintf_p(FILE *, const wchar_t *, ...);
int _fwprintf_p_l(FILE *, const wchar_t *, _locale_t, ...);
int _wprintf_l(const wchar_t *, _locale_t, ...);
int _wprintf_p(const wchar_t *, ...);
int _wprintf_p_l(const wchar_t *, _locale_t, ...);

int vfwscanf(FILE *, const wchar_t *, va_list);
int vwscanf(const wchar_t *, va_list);
int _vfwscanf_l(FILE *, const wchar_t *, _locale_t, va_list);
int _vwscanf_l(const wchar_t *, _locale_t, va_list);
int fwscanf(FILE *, const wchar_t *, ...);
int wscanf(const wchar_t *, ...);
int _fwscanf_l(FILE *, const wchar_t *, _locale_t, ...);
int _wscanf_l(const wchar_t *, _locale_t, ...);

int vswprintf(wchar_t *, size_t, const wchar_t *, va_list);
int swprintf(wchar_t *, size_t, const wchar_t *, ...);
int _vsnwprintf(wchar_t *, size_t, const wchar_t *, va_list);
int _vsnwprintf_l(wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int _snwprintf(wchar_t *, size_t, const wchar_t *, ...);
int _snwprintf_l(wchar_t *, size_t, const wchar_t *, _locale_t, ...);
int _vswprintf(wchar_t *, const wchar_t *, va_list);
int _vswprintf_l(wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int _swprintf(wchar_t *, const wchar_t *, ...);
int _swprintf_l(wchar_t *, size_t, const wchar_t *, _locale_t, ...);
int _vswprintf_p(wchar_t *, size_t, const wchar_t *, va_list);
int _vswprintf_p_l(wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int _swprintf_p(wchar_t *, size_t, const wchar_t *, ...);
int _swprintf_p_l(wchar_t *, size_t, const wchar_t *, _locale_t, ...);
int _vscwprintf(const wchar_t *, va_list);
int _vscwprintf_l(const wchar_t *, _locale_t, va_list);
int _scwprintf(const wchar_t *, ...);
int _scwprintf_l(const wchar_t *, _locale_t, ...);

int vswscanf(const wchar_t *, const wchar_t *, va_list);
int swscanf(const wchar_t *, const wchar_t *, ...);
int _vswscanf_l(const wchar_t *, const wchar_t *, _locale_t, va_list);
int _swscanf_l(const wchar_t *, const wchar_t *, _locale_t, ...);
int _vsnwscanf_l(const wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int _snwscanf(const wchar_t *, size_t, const wchar_t *, ...);
int _snwscanf_l(const wchar_t *, size_t, const wchar_t *, _locale_t, ...);

#endif
