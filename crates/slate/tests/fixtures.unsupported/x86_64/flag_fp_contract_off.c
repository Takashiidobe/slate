/* { dg-options "-ffp-contract=off -mfma" } */
int printf(const char *, ...);

int main(void) {
  volatile double base = 1.0 + 0x1p-30;
  volatile double square = 1.0 + 0x1p-29;
  double a = base;
  double residual = a * a - square;
  float fa = 1.0f + 0x1p-13f;
  float fresidual = fa * fa - (1.0f + 0x1p-12f);
  printf("%a %a\n", residual, (double)fresidual);
  return 0;
}
