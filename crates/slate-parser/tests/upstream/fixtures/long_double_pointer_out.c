#include <stdio.h>

int main(void) {
  long double parsed;
  int         matched = sscanf("0x1.0000000000000002p+0", "%La", &parsed);

  printf("%d %La\n", matched, parsed);
  return matched != 1 || parsed != 0x1.0000000000000002p0L;
}

// LOWERING: #![feature(f128)]

// REWRITES: #![feature(f128)]
