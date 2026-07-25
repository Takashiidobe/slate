#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = ' ', b = '\t', c = 'A';
  printf("%d %d %d\n", isblank(a) ? 1 : 0, isblank(b) ? 1 : 0,
         isblank(c) ? 1 : 0);
  return 0;
}
