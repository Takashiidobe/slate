#include <stdio.h>

#if defined(NDEBUG)
static int debug_code(void) { return 0; }
#else
static int debug_code(void) { return 1; }
#endif

int main(void) {
  printf("%d\n", debug_code());
  return 0;
}
