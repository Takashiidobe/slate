#include <stdio.h>
static int values[] = { 17, 29, 41 };
static int answer(void) { return 53; }
int main(void) {
  int result = 10;
  int only_negated = 0;
  int *positive;
  int *negative;
  int (*fn)(void);
  const char *text;
  __asm__("addl %1, %0\n\taddl $%n1, %0\n\taddl $%P1, %0"
          : "+r"(result) : "i"(4));
  __asm__("addl $%n1, %0" : "+r"(only_negated) : "i"(-5));
  __asm__("leaq %c1(%%rip), %0" : "=r"(positive) : "s"(&values[2]));
  __asm__("leaq %p1(%%rip), %0" : "=r"(negative) : "Ws"((char *)&values[2] - 4));
  __asm__("leaq %P1(%%rip), %0" : "=r"(fn) : "i"(answer));
  __asm__("leaq %c1(%%rip), %0" : "=r"(text) : "i"("hello"));
  printf("%d %d %d %d %s\n", result + only_negated, *positive, *negative, fn(), text);
  return 0;
}
