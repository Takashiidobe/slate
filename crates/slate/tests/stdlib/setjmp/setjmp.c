#include <setjmp.h>
#include <stdio.h>
static jmp_buf buf;
static void jump(int n) { longjmp(buf, n); }
int main(void) {
  int r = setjmp(buf);
  if (r == 0) {
    printf("before\n");
    jump(42);
    printf("unreachable\n");
  }
  printf("after r=%d\n", r);
  return 0;
}
