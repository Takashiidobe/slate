#include <wctype.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(WEOF == (wint_t)0xffff, "WEOF");
_Static_assert(_UPPER == 0x01, "_UPPER");
_Static_assert(_LOWER == 0x02, "_LOWER");
_Static_assert(_DIGIT == 0x04, "_DIGIT");
_Static_assert(_SPACE == 0x08, "_SPACE");
_Static_assert(_PUNCT == 0x10, "_PUNCT");
_Static_assert(_CONTROL == 0x20, "_CONTROL");
_Static_assert(_BLANK == 0x40, "_BLANK");
_Static_assert(_HEX == 0x80, "_HEX");
_Static_assert(_LEADBYTE == 0x8000, "_LEADBYTE");
_Static_assert(_ALPHA == 0x0103, "_ALPHA");

TYPE_IS(&__pctype_func, const unsigned short *(*)(void));
TYPE_IS(&__pwctype_func, const wctype_t *(*)(void));
TYPE_IS(_pctype, const unsigned short *);
TYPE_IS(_pwctype, const wctype_t *);

TYPE_IS(&iswalnum, int (*)(wint_t));
TYPE_IS(&iswalpha, int (*)(wint_t));
TYPE_IS(&iswascii, int (*)(wint_t));
TYPE_IS(&iswblank, int (*)(wint_t));
TYPE_IS(&iswcntrl, int (*)(wint_t));
TYPE_IS(&iswdigit, int (*)(wint_t));
TYPE_IS(&iswgraph, int (*)(wint_t));
TYPE_IS(&iswlower, int (*)(wint_t));
TYPE_IS(&iswprint, int (*)(wint_t));
TYPE_IS(&iswpunct, int (*)(wint_t));
TYPE_IS(&iswspace, int (*)(wint_t));
TYPE_IS(&iswupper, int (*)(wint_t));
TYPE_IS(&iswxdigit, int (*)(wint_t));
TYPE_IS(&__iswcsymf, int (*)(wint_t));
TYPE_IS(&__iswcsym, int (*)(wint_t));

TYPE_IS(&_iswalnum_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswalpha_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswblank_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswcntrl_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswdigit_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswgraph_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswlower_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswprint_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswpunct_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswspace_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswupper_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswxdigit_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswcsymf_l, int (*)(wint_t, _locale_t));
TYPE_IS(&_iswcsym_l, int (*)(wint_t, _locale_t));
TYPE_IS(&iswctype, int (*)(wint_t, wctype_t));
TYPE_IS(&_iswctype_l, int (*)(wint_t, wctype_t, _locale_t));
TYPE_IS(&towupper, wint_t (*)(wint_t));
TYPE_IS(&towlower, wint_t (*)(wint_t));
TYPE_IS(&_towupper_l, wint_t (*)(wint_t, _locale_t));
TYPE_IS(&_towlower_l, wint_t (*)(wint_t, _locale_t));
TYPE_IS(&towctrans, wint_t (*)(wint_t, wctrans_t));
TYPE_IS(&wctrans, wctrans_t (*)(const char *));
TYPE_IS(&wctype, wctype_t (*)(const char *));

extern int iswalnum_l;
extern int towlower_l;
extern int towctrans_l;
extern int wctrans_l;
extern int wctype_l;

int main(void) { return 0; }
