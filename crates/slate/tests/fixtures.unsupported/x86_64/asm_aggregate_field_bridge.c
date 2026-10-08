#include <stdio.h>
struct padded { unsigned char first; unsigned short second; };
struct flags { _Bool first; unsigned char second; };
int main(void) {
  struct padded padded = { 1, 2 };
  struct flags flags = { 1, 2 };
  __asm__("" : "+r"(padded));
  __asm__("" : "+r"(flags));
  printf("%u %u %d %u\n", padded.first, padded.second, flags.first, flags.second);
  return 0;
}
