#include <stdlib.h>
#include <stdio.h>
int main(void) {
  wchar_t wbuf[16];
  size_t n = mbstowcs(wbuf, "hello", 16);
  char cbuf[16];
  size_t m = wcstombs(cbuf, wbuf, 16);
  printf("%zu %zu %s\n", n, m, cbuf);
  return 0;
}
