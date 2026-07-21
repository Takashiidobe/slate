#include "test.h"

// Test pointer parameter without name
void *foo(void *);

void *foo(void *p) {
  return p;
}

int main() {
  printf("OK\n");
  return 0;
}
