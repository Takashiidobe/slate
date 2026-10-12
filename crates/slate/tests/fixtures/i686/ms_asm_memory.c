/* { dg-options "-fms-extensions" } */
#include <stdio.h>

static int add_mem(int a, int b) {
  int result;
  __asm {
    mov eax, a
    add eax, b
    mov result, eax
  }
  return result;
}

static int field_and_index(void) {
  struct { int x; int y; } s = { 3, 4 };
  int arr[4] = { 10, 20, 30, 40 };
  int out;
  __asm {
    mov eax, s.y
    add eax, arr[8]
    mov out, eax
  }
  return out;
}

static int sized(void) {
  unsigned char bytes[4] = { 1, 2, 3, 4 };
  unsigned out;
  __asm {
    movzx eax, byte ptr bytes[2]
    mov out, eax
  }
  return (int)out;
}

static int indexed(int i) {
  int arr[4] = { 5, 6, 7, 8 };
  int out;
  __asm {
    mov ecx, i
    mov eax, arr[ecx*4]
    mov out, eax
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", add_mem(2, 5), field_and_index(), sized(), indexed(2));
  return 0;
}
