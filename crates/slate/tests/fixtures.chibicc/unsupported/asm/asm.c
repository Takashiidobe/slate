#include "test.h"

__attribute__((naked)) char *asm_fn1(void) {
  asm("mov $50, %rax\n\t"
      "ret");
}

__attribute__((naked)) char *asm_fn2(void) {
  asm inline volatile("mov $55, %rax\n\t"
                      "ret");
}

int main() {
  ASSERT(50, (int)asm_fn1());
  ASSERT(55, (int)asm_fn2());

  printf("OK\n");
  return 0;
}
