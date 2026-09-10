#include <stdio.h>

#ifdef MY_FEATURE
int feature_code(void) { return 10; }
#else
int feature_code(void) { return 20; }
#endif

int main(void) {
  printf("%d\n", feature_code());
  return 0;
}
