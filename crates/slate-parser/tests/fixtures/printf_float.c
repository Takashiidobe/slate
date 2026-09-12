#include <stdio.h>

int main(void) {
  float  x = 1.5f;
  double y = 2.25;
  printf("%f %.2f %.0f\n", x + 0.5f, y * 2.0, 3.0);
  printf("tail %.3f", 1.25);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
