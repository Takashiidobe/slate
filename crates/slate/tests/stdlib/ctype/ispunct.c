#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = 'A', b = ' ';
  printf("%d %d\n", ispunct(a) ? 1 : 0, ispunct(b) ? 1 : 0);
  return 0;
}
