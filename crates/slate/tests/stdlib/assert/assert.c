#include <assert.h>
#include <stdio.h>

_Static_assert(__STDC_VERSION_ASSERT_H__ == 202311L,
               "C23 assert header version");

int main(void) {
  volatile int evaluations = 0;
  assert(++evaluations == 1);
  assert((int[2]){3, 4}[1] == 4);
  printf("%d\n", evaluations);
  return 0;
}
