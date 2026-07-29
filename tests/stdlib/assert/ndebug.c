#define NDEBUG
#include <assert.h>
#include <stdio.h>

int main(void) {
  volatile int evaluations = 0;
  assert(++evaluations == 1);
  printf("%d\n", evaluations);
  return 0;
}
