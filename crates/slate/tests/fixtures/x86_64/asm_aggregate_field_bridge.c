#include <stdio.h>
struct padded { unsigned char first; unsigned short second; };
struct flags { _Bool first; unsigned char second; };
struct bits { unsigned low : 3; unsigned high : 7; unsigned short tail; };
union partial { unsigned char byte; unsigned int word; };
struct nested { struct flags inner; _Bool done[2]; };
struct wide { char tag; int value; };
int main(void) {
  struct padded padded = { 1, 2 };
  struct flags flags = { 1, 2 };
  __asm__("" : "+r"(padded));
  __asm__("" : "+r"(flags));
  printf("%u %u %d %u\n", padded.first, padded.second, flags.first, flags.second);
  __asm__("addl $0x00030000, %0" : "+r"(padded));
  printf("%u %u\n", padded.first, padded.second);
  struct flags set;
  __asm__("movw $0x2a00, %w0\n\tcmpb $1, %b1\n\tsete %b0" : "=&r"(set) : "r"(5));
  printf("%d %u\n", set.first, set.second);
  struct bits bits = { 5, 100, 7 };
  __asm__("xorl $0x00010009, %0" : "+r"(bits));
  printf("%u %u %u\n", bits.low, bits.high, bits.tail);
  union partial partial;
  partial.word = 0;
  partial.byte = 9;
  unsigned int seen;
  __asm__("movl %1, %0" : "=r"(seen) : "r"(partial));
  printf("%u\n", seen);
  __asm__("movl $0x01020304, %0" : "=r"(partial));
  printf("%u %u\n", partial.byte, partial.word);
  struct nested nested = { { 0, 3 }, { 1, 0 } };
  __asm__("xorl $0x01000101, %0" : "+r"(nested));
  printf("%d %u %d %d\n", nested.inner.first, nested.inner.second, nested.done[0],
         nested.done[1]);
  struct wide wide = { 'a', -7 };
  struct wide copy;
  __asm__("movq %1, %0\n\taddq $1, %0" : "=r"(copy) : "r"(wide));
  printf("%c %d\n", copy.tag, copy.value);
  return 0;
}
