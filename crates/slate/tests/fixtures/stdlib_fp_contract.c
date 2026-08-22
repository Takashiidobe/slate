#include <stdio.h>

int main(void) {
  volatile double x = 0x1.fffffffffffffp+0;
  double y = x;
  double z = -(x * x);
  double contracted;
  double uncontracted;

  {
#pragma STDC FP_CONTRACT ON
    contracted = x * y + z;
  }

  {
#pragma STDC FP_CONTRACT OFF
    uncontracted = x * y + z;
  }

  printf("%.20e %.20e\n", contracted, uncontracted);
  return 0;
}
