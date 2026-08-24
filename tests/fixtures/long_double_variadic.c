#include <stdio.h>

int main(void) {
  long double x = 0x1.0000000000000001p0L;
  long double y;
  printf("%La\n", x);
  printf("%.21Lf\n", x);
  if (sscanf("0x1.0000000000000001p+0", "%La", &y) == 1)
    printf("%La\n", y);
  return 0;
}
// LOWERING-DAG: fn __slate_printf__ri32_pi8_f80(
// LOWERING-DAG: fn __slate_sscanf__ri32_pi8_pi8_pf80(
