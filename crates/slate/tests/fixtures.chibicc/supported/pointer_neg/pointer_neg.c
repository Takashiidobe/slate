#include "test.h"

int main() {
  ASSERT(5, ({ int x[2]; x[1]=5; *(&x[0]-(-1)); }));

  printf("OK\n");
  return 0;
}
