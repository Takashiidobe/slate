#ifndef _SLATE_WCTYPE_H
#define _SLATE_WCTYPE_H

#define __need_wint_t
#include <stddef.h>
#undef __need_wint_t

int    iswalpha(wint_t wc);
int    iswdigit(wint_t wc);
int    iswspace(wint_t wc);
int    iswupper(wint_t wc);
wint_t towlower(wint_t wc);
wint_t towupper(wint_t wc);

#endif
