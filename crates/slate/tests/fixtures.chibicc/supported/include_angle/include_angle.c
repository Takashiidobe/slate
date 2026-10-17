#include "test.h"

#include <include3.h>

int main() {
  ASSERT(3, foo);

#include <include4.h>
  ASSERT(4, foo);

  printf("OK\n");
  return 0;
}
