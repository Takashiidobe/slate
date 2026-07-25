#include <stdio.h>

int main(void) {
  double x = 1234.5678;
  int prec = 2;
  printf("%015.2e\n", x);
  printf("%.*e\n", prec, x);
  printf("% e\n", x);
  printf("%#e\n", x);
  return 0;
}
