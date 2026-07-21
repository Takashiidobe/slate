#include "test.h"

int main() {
  int m = 0;

#if no_such_symbol == 0
  m = 5;
#else
  m = 6;
#endif
  ASSERT(5, m);

#define FOO bar
#if FOO == 0
  m = 7;
#else
  m = 8;
#endif
  ASSERT(7, m);

  printf("OK\n");
  return 0;
}
