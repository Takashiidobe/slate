#include <stdio.h>

static int read_via_memory(int x) {
  int result;
  __asm__("movl %1, %0" : "=r"(result) : "m"(x));
  return result;
}

static int increment_in_place(int x) {
  __asm__("incl %0" : "+m"(x));
  return x;
}

static int store_via_memory(void) {
  int x;
  __asm__("movl $42, %0" : "=m"(x));
  return x;
}

int main(void) {
  printf("%d %d %d\n", read_via_memory(7), increment_in_place(9),
         store_via_memory());
  return 0;
}


