#include <stdio.h>

double sum_args(int wide, int count, ...);

int main(void) {
  printf("%.3f\n", sum_args(0, 2, 1.25, 2.5));
  printf("%.3f\n", sum_args(0, 3, 1.25, 2.5, 3.125));
  return 0;
}
