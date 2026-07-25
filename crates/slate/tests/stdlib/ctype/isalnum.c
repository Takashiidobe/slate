#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = 'A', b = ' ';
  printf("%d %d\n", isalnum(a) ? 1 : 0, isalnum(b) ? 1 : 0);
  return 0;
}
