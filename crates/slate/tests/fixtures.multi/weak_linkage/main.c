#include <stdio.h>

// LOWERING-DAG: #![feature(linkage)]
// REWRITES-DAG: #![feature(linkage)]

int        fallback_value(void);
extern int weak_global;

int main(void) {
  printf("%d %d\n", fallback_value(), weak_global);
  return 0;
}
