#include <stdio.h>
#include <stdlib.h>
int main(void) {
  char *end = 0;
  double d = strtod("3.25xyz", &end);
  printf("%.2f %c\n", d, *end);
  return 0;
}
