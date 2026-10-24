/* { dg-options "-fopenmp-simd" } */
int printf(const char *, ...);

int main(void) {
  int values[64];
  for (int i = 0; i < 64; i++)
    values[i] = i * 3;
  long sum = 0;
#pragma omp simd reduction(+ : sum)
  for (int i = 0; i < 64; i++)
    sum += values[i];
#ifdef _OPENMP
  int openmp = 1;
#else
  int openmp = 0;
#endif
  printf("%ld %d\n", sum, openmp);
  return 0;
}
