#ifndef _SLATE_WCHAR_H
#define _SLATE_WCHAR_H

#define __need_wchar_t
#define __need_size_t
#define __need_wint_t
#define __need_NULL
#include <stddef.h>
#undef __need_wchar_t
#undef __need_size_t
#undef __need_wint_t
#undef __need_NULL

#define WEOF ((wint_t) - 1)

wchar_t *wcschr(const wchar_t *s, wchar_t c);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
size_t wcslen(const wchar_t *s);

#endif
