#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "C");
  int lower = 'q';
  int upper = 'Q';
  printf("%d %d\n", toupper(lower), tolower(upper));
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
