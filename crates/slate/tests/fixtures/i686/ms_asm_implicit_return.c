/* { dg-options "-fms-extensions" } */
#include <stdio.h>

static int implicit_return(int v) {
  __asm mov eax, v
  __asm shl eax, 1
}

static unsigned long long implicit_return64(void) {
  __asm {
    mov eax, 0x11223344
    mov edx, 0x55667788
  }
}

static unsigned char narrow(int v) {
  __asm mov eax, v
}

static short narrow16(int v) {
  __asm {
    mov eax, v
    add eax, 1
  }
}

static int global_value = 5;

static int *pointer(void) {
  __asm lea eax, global_value
}

static int explicit_wins(int v) {
  __asm mov eax, 99
  return v;
}

int main(void) {
  printf("%d %llx %u %d\n", implicit_return(21), implicit_return64(), narrow(0x1ff), narrow16(0x7fff));
  printf("%d %d\n", *pointer(), explicit_wins(3));
  return 0;
}
