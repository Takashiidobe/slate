#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *end = 0;
  long double d = strtold("2.75tail", &end);
  printf("%.2Lf %c\n", d, *end);
  return 0;
}
