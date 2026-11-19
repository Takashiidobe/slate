/* { dg-options "-msseregparm" } */
#include <stdio.h>

double scale(double x, int n) { return x * n; }

int main(void) {
  printf("%.2f\n", scale(1.25, 3));
  return 0;
}
