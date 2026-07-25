#include <stdio.h>
#include <wctype.h>
int main(void) {
  printf("%d%d%d%d%d\n", iswalpha(L'a') != 0, iswdigit(L'7') != 0,
         iswspace(L' ') != 0, iswupper(L'Z') != 0, iswalpha(L'5') != 0);
  printf("%c%c\n", (int)towupper(L'a'), (int)towlower(L'A'));
  return 0;
}
