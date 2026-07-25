#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = ' ', b = 'A', c = '\n';
  printf("%d %d %d\n", isprint(a) ? 1 : 0, isprint(b) ? 1 : 0,
         isprint(c) ? 1 : 0);
  return 0;
}
