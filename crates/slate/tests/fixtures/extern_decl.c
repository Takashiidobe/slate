#include <stdio.h>

int toupper(int c);
int tolower(int c);
int abs(int n);

int main(void) {
  printf("%c%c %d\n", toupper('a'), tolower('Z'), abs(-42));
  return 0;
}
