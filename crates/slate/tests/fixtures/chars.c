#include <stdio.h>

static char add_char(char a, char b) {
  char c = a + b;
  return c;
}

int main(void) {
  char x = 10;
  unsigned char y = 200;
  signed char z = -5;
  char letter = 'A';
  printf("%d\n", add_char(x, z));
  printf("%d\n", y);
  printf("%c\n", letter);
  printf("%c\n", letter + 1);
  return 0;
}
