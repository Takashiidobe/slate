#ifndef _SLATE_XLOCALE_H
#define _SLATE_XLOCALE_H

#if defined(__SLATE_LIBC_DARWIN)

#include <locale.h>

#define LC_C_LOCALE ((locale_t)0)

extern const locale_t _c_locale;

struct lconv *localeconv_l(locale_t);
const char   *querylocale(int, locale_t);

#elif defined(__SLATE_LIBC_FREEBSD)

#include <locale.h>

struct lconv *localeconv_l(locale_t);

#endif

#endif
