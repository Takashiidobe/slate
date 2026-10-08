#include <fenv.h>
#include <stdio.h>

int main(void) {
  int original = fegetround();
  int modes[] = {FE_TONEAREST, FE_TOWARDZERO, FE_UPWARD, FE_DOWNWARD};
  for (unsigned i = 0; i < 4; ++i) {
    if (fesetround(modes[i])) return 1;
    printf("%d\n", __builtin_flt_rounds());
  }
  if (fesetround(original)) return 2;
  printf("%d\n", __builtin_flt_rounds());
  return 0;
}
