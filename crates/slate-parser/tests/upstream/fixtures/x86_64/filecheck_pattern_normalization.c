#include <stdio.h>

int main(void) {
  int left;
  int right;
  __asm__("movl $3, %0\n\tmovl $4, %1" : "=r"(left), "=r"(right));
  printf("%d _v9 anon_4 anon_struct_i32\n", left * 10 + right);
  return 0;
}

// LOWERING-X86_64-GNU: #![feature(c_variadic)]

// REWRITES-X86_64-GNU: #![feature(c_variadic)]
