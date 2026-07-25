#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = 'A', b = ' ';
  printf("%d %d\n", isupper(a) ? 1 : 0, isupper(b) ? 1 : 0);
  return 0;
}
