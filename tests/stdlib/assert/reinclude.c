#include <assert.h>
#include <stdio.h>

int main(void) {
  volatile int evaluations = 0;
  assert(++evaluations == 1);

#define NDEBUG
#include <assert.h>
  assert(++evaluations == 2);

#undef NDEBUG
#include <assert.h>
  assert(++evaluations == 2);

  printf("%d\n", evaluations);
  return 0;
}
