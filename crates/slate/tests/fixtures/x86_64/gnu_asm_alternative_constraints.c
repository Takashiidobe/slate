#include <stdio.h>

static int add_ri(int a, int b) {
  int result;
  __asm__("addl %2, %0" : "=r"(result) : "0"(a), "ri"(b) : "cc");
  return result;
}

static int add_rm(int a, int *p) {
  int result;
  __asm__("addl %2, %0" : "=r"(result) : "0"(a), "rm"(*p) : "cc");
  return result;
}

int main(void) {
  int x = 4;
  printf("%d %d\n", add_ri(3, 5), add_rm(3, &x));
  return 0;
}
