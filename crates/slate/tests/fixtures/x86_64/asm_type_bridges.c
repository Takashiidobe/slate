#include <stdio.h>
struct word { unsigned int value; };
union half { unsigned short value; };
enum number { THREE = 3 };
static int answer(void) { return 42; }
int main(void) {
  _Bool flag = 1;
  signed char byte = -2;
  unsigned int tied = 0;
  int input = -4;
  enum number number = THREE;
  struct word word = { 40 };
  union half half = { 9 };
  int (*fn)(void) = answer;
  __asm__("incb %0" : "+r"(byte));
  __asm__("incl %0" : "=r"(tied) : "0"(input));
  __asm__("xorb $1, %0" : "+r"(flag));
  __asm__("incl %0" : "+r"(number));
  __asm__("addl $2, %0" : "+r"(word));
  __asm__("incw %0" : "+r"(half));
  __asm__("" : "+r"(fn));
  printf("%d %d %u %u %d\n", flag, number, word.value, half.value, fn());
  fn = 0;
  __asm__("" : "+r"(fn));
  printf("%d %d %u\n", fn == 0, byte, tied);
  return 0;
}
