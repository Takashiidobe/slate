#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile a = '\n', b = 0, c = 'A';
  printf("%d %d %d\n", iscntrl(a) ? 1 : 0, iscntrl(b) ? 1 : 0,
         iscntrl(c) ? 1 : 0);
  return 0;
}
