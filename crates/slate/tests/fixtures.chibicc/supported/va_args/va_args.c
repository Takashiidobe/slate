#include "test.h"

int add2(int x, int y) { return x + y; }

int add6(int a, int b, int c, int d, int e, int f) {
  return a + b + c + d + e + f;
}

int main() {
#define M14(...) 3
  ASSERT(3, M14());

#define M14(...) __VA_ARGS__
  ASSERT(2, M14() 2);
  ASSERT(5, M14(5));

#define M14(...) add2(__VA_ARGS__)
  ASSERT(8, M14(2, 6));

#define M14(...) add6(1,2,__VA_ARGS__,6)
  ASSERT(21, M14(3,4,5));

#define M14(x, ...) add6(1,2,x,__VA_ARGS__,6)
  ASSERT(21, M14(3,4,5));

#define M14(x, ...) x
  ASSERT(5, M14(5));

  printf("OK\n");
  return 0;
}
