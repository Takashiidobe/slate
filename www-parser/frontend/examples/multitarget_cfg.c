#include <stdio.h>
#include <sys/syscall.h>

#ifdef MY_FEATURE
int feature_code(void) { return 10; }
#else
int feature_code(void) { return 20; }
#endif

int main(void) {
  printf("%d %d\n", feature_code(), (int)SYS_preadv);
  return 0;
}
