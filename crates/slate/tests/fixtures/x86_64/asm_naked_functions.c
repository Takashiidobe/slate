#include <stdio.h>

__attribute__((naked)) int identity(int x) {
  __asm__("movl %edi, %eax\n\tret");
}

__attribute__((naked)) long add(long a, long b) {
  __asm__("leaq (%rdi,%rsi), %rax");
  ;
  __asm__("ret");
}

__attribute__((naked)) static int forty_two(void) {
  __asm__ volatile("movl %0, %%eax\n\tret" : : "i"(40 + 2));
}

__attribute__((naked)) int declared(void);

int declared(void) {
  __asm__("movl $5, %eax\n\tret # {braces}");
}

int main(void) {
  int (*fp)(int) = identity;
  printf("%d %ld %d %d %d\n", identity(9), add(30, 12), forty_two(), declared(), fp(3));
  return 0;
}
