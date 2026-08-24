#include <errno.h>
#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#include <xlocale.h>

struct slate_macos_stdio_locale_layouts {
  FILE      stream;
  fpos_t    position;
  mbstate_t conversion;
  struct lconv numeric;
  locale_t  locale;
  wint_t    wide;
  wctype_t  classification;
  wctrans_t transformation;
};

FILE *slate_macos_open(const char *path) { return fopen(path, "r"); }

int slate_macos_errno(void) { return errno; }

FILE *slate_macos_stdin(void) { return stdin; }

int slate_macos_classify(wint_t value, locale_t locale) {
  return iswctype_l(value, wctype("alpha"), locale);
}

int slate_macos_export(void) { return LC_ALL + CODESET; }
