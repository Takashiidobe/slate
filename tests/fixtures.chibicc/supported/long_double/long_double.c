#include "test.h"

double to_double(long double x) {
  return x;
}

long double to_ldouble(int x) {
  return x;
}

int main() {
  ASSERT(3, (long double)3);
  ASSERT(5, (long double)3 + 2);
  ASSERT(6, (long double)3 * 2);
  ASSERT(5, (long double)3 + 2.0);

  ASSERT(0, ({
    char buf[100];
    sprintf(buf, "%Lf", (long double)12.3);
    strncmp(buf, "12.3", 4);
  }));

  ASSERT(1, to_double(3.5) == 3.5);
  ASSERT(0, to_double(3.5) == 3);

  ASSERT(1, (long double)5.0 == (long double)5.0);
  ASSERT(0, (long double)5.0 == (long double)5.2);

  ASSERT(1, to_ldouble(5.0) == 5.0);
  ASSERT(0, to_ldouble(5.0) == 5.2);

  printf("OK\n");
  return 0;
}
