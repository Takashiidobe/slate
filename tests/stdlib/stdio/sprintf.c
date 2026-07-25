#include <stdio.h>
int main(void) {
  char b[32];
  sprintf(b, "%d-%d", 7, 42);
  puts(b);
  return 0;
}
