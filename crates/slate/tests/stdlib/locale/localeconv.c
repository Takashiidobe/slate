#include <locale.h>
#include <stdio.h>
int main(void) {
  setlocale(LC_ALL, "C");
  struct lconv *lc = localeconv();
  printf("[%s] [%s] cur=%d\n", lc->decimal_point, lc->thousands_sep,
         lc->frac_digits);
  return 0;
}
