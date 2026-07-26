#include <stdio.h>

static int gnu_asm_goto(int value) {
  __asm__ goto("testl %[value], %[value]\n\tjz %l[zero]"
               :
               : [value] "r"(value)
               : "cc"
               : zero);
  return 11;
zero:
  return 13;
}

static int gnu_asm_goto_output(int value) {
  int output = value;
  __asm__ goto("testl %[output], %[output]\n\tjz %l[zero]"
               : [output] "+r"(output)
               :
               : "cc"
               : zero);
  return output + 1;
zero:
  return output + 2;
}

int main(void) {
  printf("%d %d %d\n", gnu_asm_goto(0), gnu_asm_goto_output(5),
         gnu_asm_goto_output(0));
  return 0;
}
