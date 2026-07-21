#include "test.h"

int main() {
  typedef struct {
    int a : 10;
    int b : 10;
    int c : 10;
  } T3;

  ASSERT(1, ({ T3 x={1,2,3}; x.a++; }));
  ASSERT(2, ({ T3 x={1,2,3}; x.b++; }));
  ASSERT(3, ({ T3 x={1,2,3}; x.c++; }));

  ASSERT(2, ({ T3 x={1,2,3}; ++x.a; }));
  ASSERT(3, ({ T3 x={1,2,3}; ++x.b; }));
  ASSERT(4, ({ T3 x={1,2,3}; ++x.c; }));

  printf("OK\n");
  return 0;
}
