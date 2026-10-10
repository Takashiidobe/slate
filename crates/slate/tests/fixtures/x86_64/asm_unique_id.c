#include <stdio.h>

static int count_down(int value) {
  int steps = 0;
  __asm__(".Lloop%=:\n\t"
          "testl %1, %1\n\t"
          "jz .Ldone%=\n\t"
          "decl %1\n\t"
          "incl %0\n\t"
          "jmp .Lloop%=\n"
          ".Ldone%=:"
          : "+r"(steps), "+r"(value)
          :
          : "cc");
  return steps;
}

static inline __attribute__((always_inline)) unsigned clamp_ten(unsigned value) {
  __asm__("cmpl $10, %0\n\t"
          "jbe skip_%=\n\t"
          "movl $10, %0\n"
          "skip_%=:"
          : "+r"(value)
          :
          : "cc");
  return value;
}

static unsigned clamp_sum(unsigned limit) {
  unsigned sum = 0;
  for (unsigned i = 0; i < limit; i++) {
    sum += clamp_ten(i) + clamp_ten(i * 3);
  }
  return sum;
}

static int first_char(void) {
  const char *text;
  __asm__(".pushsection .rodata\n"
          "str_%=: .asciz \"slate\"\n"
          ".popsection\n\t"
          "leaq str_%=(%%rip), %0"
          : "=r"(text));
  return text[0] + text[4];
}

static int with_own_label(int value) {
  __asm__("testl %0, %0\n\t"
          "jns 2f\n\t"
          "negl %0\n"
          "2:\n\t"
          "cmpl $100, %0\n\t"
          "jb small%=\n\t"
          "movl $100, %0\n"
          "small%=:"
          : "+r"(value)
          :
          : "cc");
  return value;
}

int main(void) {
  printf("%d %d %u %d %d %d\n", count_down(0), count_down(7), clamp_sum(16),
         first_char(), with_own_label(-42), with_own_label(500));
  return 0;
}
