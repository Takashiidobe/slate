#include <stdio.h>

static int inc_byte_view_of_int(int x) {
  __asm__("incb %b0" : "+r"(x));
  return x;
}

static int inc_word_view_of_int(int x) {
  __asm__("incw %w0" : "+r"(x));
  return x;
}

static long inc_dword_view_of_long(long x) {
  __asm__("incl %k0" : "+r"(x));
  return x;
}

static long inc_qword_view_of_long(long x) {
  __asm__("incq %q0" : "+r"(x));
  return x;
}

int main(void) {
  printf("%d %d %ld %ld\n", inc_byte_view_of_int(0xFF), inc_word_view_of_int(0xFFFF),
         inc_dword_view_of_long(1), inc_qword_view_of_long(1));
  return 0;
}
