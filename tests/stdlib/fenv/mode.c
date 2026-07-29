#include <fenv.h>
#include <stdio.h>

int main(void) {
  femode_t original_mode;

  printf("femode_t %zu\n", sizeof(femode_t));
  printf("fegetmode %d\n", fegetmode(&original_mode));
  printf("fesetround %d\n", fesetround(FE_DOWNWARD));
  printf("fesetmode default %d\n", fesetmode(FE_DFL_MODE));
  printf("default mode %d\n", fegetround() == FE_TONEAREST);
  printf("fesetmode saved %d\n", fesetmode(&original_mode));
  return 0;
}
