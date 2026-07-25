#include <stdio.h>
int main(void) {
  int x = 255;
  unsigned u = 42u;
  long long ll = 1234567890123LL;
  double d = 1.25;
  printf("%x %u %lld %.2f\n", x, u, ll, d);
  return 0;
}
