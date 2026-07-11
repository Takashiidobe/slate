#include <locale.h>
#include <string.h>
#include <stdio.h>
int main(void) {
  char out[16];
  setlocale(LC_ALL, "C");
  size_t n = strxfrm(out, "abc", sizeof out);
  printf("%zu %d %d\n", n, strcmp(out, "abc") == 0, strxfrm(out, "abcdef", 4) >= 6);
  return 0;
}
