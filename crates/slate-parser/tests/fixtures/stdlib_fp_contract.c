#include <stdio.h>

int main(void) {
  volatile double x = 0x1.fffffffffffffp+0;
  double          y = x;
  double          z = -(x * x);
  double          contracted;
  double          uncontracted;

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

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
