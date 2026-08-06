#include <stdlib.h>

int main(void) {
  int *p  = malloc(sizeof(int) * 2);
  p[0]    = 1;
  p[1]    = 2;
  int sum = p[0] + p[1];
  free(p);
  return sum;
}
