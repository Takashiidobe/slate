#include <stdio.h>

int main(void) {
  __auto_type   x = 5;
  __typeof__(x) y = x + 2;
  printf("%d\n", y);
  return y;
}




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: target builtin type
// SLATE-FILECHECK-END DEFAULT
