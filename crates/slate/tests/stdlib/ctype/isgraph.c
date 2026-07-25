#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = 'A', b = ' ', c = '\n';
  printf("%d %d %d\n", isgraph(a) ? 1 : 0, isgraph(b) ? 1 : 0,
         isgraph(c) ? 1 : 0);
  return 0;
}
