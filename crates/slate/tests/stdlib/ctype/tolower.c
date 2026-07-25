#include <ctype.h>
#include <stdio.h>
int main(void) {
  int volatile c = 'A';
  printf("%c\n", tolower(c));
  return 0;
}
