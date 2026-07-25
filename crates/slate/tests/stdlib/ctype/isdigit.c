#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = 'A', b = ' ';
  printf("%d %d\n", isdigit(a) ? 1 : 0, isdigit(b) ? 1 : 0);
  return 0;
}
