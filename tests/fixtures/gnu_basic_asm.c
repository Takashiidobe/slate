#include <stdio.h>

__asm__(".text\n"
        ".globl gnu_basic_asm_function\n"
        ".type gnu_basic_asm_function,@function\n"
        "gnu_basic_asm_function:\n"
        "movl $37, %eax\n"
        "ret\n"
        ".size gnu_basic_asm_function,.-gnu_basic_asm_function\n");

extern int gnu_basic_asm_function(void);

static int gnu_basic_asm_value;

static int gnu_function_basic_asm(void) {
  __asm__ volatile("movl $23, gnu_basic_asm_value(%rip)");
  return gnu_basic_asm_value;
}

int main(void) {
  printf("%d %d\n", gnu_basic_asm_function(), gnu_function_basic_asm());
  return 0;
}
