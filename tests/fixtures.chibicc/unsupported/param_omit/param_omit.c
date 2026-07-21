#include "test.h"

// Function declarations with omitted parameter names
int add(int, int);
int mul(int, int);
void *memcpy(void *, const void *, unsigned long);

// Function definitions (parameter names are required)
int add(int x, int y) {
  return x + y;
}

int mul(int a, int b) {
  return a * b;
}

// Forward declaration with omitted names, definition with names
int sub(int, int);
int sub(int x, int y) {
  return x - y;
}

int main() {
  ASSERT(5, add(2, 3));
  ASSERT(6, mul(2, 3));
  ASSERT(1, sub(3, 2));

  printf("OK\n");
  return 0;
}
