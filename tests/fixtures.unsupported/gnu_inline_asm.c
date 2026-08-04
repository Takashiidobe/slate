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

static int gnu_extended_asm(int input) {
  int output;
  int tied;
  int early;
  int memory = 5;
  int zero;

  __asm__ volatile("leal 3(%1), %0" : "=r"(output) : "r"(input) : "cc");
  __asm__ volatile("addl $2, %0" : "+r"(output) : : "cc");
  __asm__("imull %[right], %[result]"
          : [result] "=r"(tied)
          : "0"(output), [right] "r"(2)
          : "cc");
  __asm__("movl %[left], %[result]\n\taddl %[right], %[result]"
          : [result] "=&r"(early)
          : [left] "r"(tied), [right] "r"(1)
          : "cc");
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(early)
          : [amount] "i"(4)
          : "cc");
  __asm__ volatile("incl %[slot]" : [slot] "+m"(memory) : : "cc", "memory");
  __asm__("testl %[value], %[value]"
          : "=@ccz"(zero)
          : [value] "r"(input)
          : "cc");
  return early + memory + zero;
}

static int gnu_numeric_operands(int left, int right) {
  int result;
  __asm__("subl %2, %0" : "=r"(result) : "0"(left), "r"(right) : "cc");
  return result;
}

static int gnu_local_label(void) {
  __asm__ volatile("jmp 1f\n\t1:\n\tnop" : : : "memory");
  __asm__ inline volatile("" : : : "memory");
  return 17;
}

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
  printf("%d %d %d %d %d %d %d %d\n", gnu_basic_asm_function(),
         gnu_function_basic_asm(), gnu_extended_asm(7),
         gnu_numeric_operands(19, 4), gnu_local_label(), gnu_asm_goto(0),
         gnu_asm_goto_output(5), gnu_asm_goto_output(0));
  return 0;
}
