/* { dg-options "-fopenmp" } */
#include <stdio.h>

int main(void) {
  long sum = 0;
#pragma omp parallel for reduction(+ : sum) num_threads(4)
  for (int i = 1; i <= 1000; i++)
    sum += i;
  printf("%ld %d\n", sum, _OPENMP >= 201511);
  return 0;
}
