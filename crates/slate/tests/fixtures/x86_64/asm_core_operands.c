#include <stdio.h>

static int core(int a, int b) {
  int output, tied, early;
  __asm__ volatile("leal 3(%1), %0" : "=r"(output) : "r"(a) : "cc");
  __asm__ volatile("addl $2, %0" : "+r"(output) : : "cc");
  __asm__("imull %2, %0" : "=r"(tied) : "0"(output), "r"(b) : "cc");
  __asm__("movl %1, %0\n\taddl %2, %0" : "=&r"(early) : "r"(tied), "r"(1) : "cc");
  __asm__("addl $1, %0" : "+&r"(early) : : "cc");
  return early;
}

int main(void) {
  int x = 3;
  int *p = &x;
  volatile int y = 4;
  __asm__ volatile("addl $2, %0" : "+r"(*p++) : : "memory", "cc");
  __asm__ volatile("addl $3, %0" : "+r"(y) : : "cc");
  __asm__ volatile("movl $19, %%eax; movl %%eax, %0" : "=r"(x) : : "eax", "cc");
  __asm__ volatile("# %% literal %{braces%}" : : : "memory");
  __asm__ volatile("nop");
  __asm__ volatile("" : : : "unwind");
  int constant;
  __asm__("movl %1, %0" : "=r"(constant) : "i"(7));
  printf("%d %d %d %d %ld\n", core(7, 2), x, y, constant, (long)(p - &x));
  return 0;
}
