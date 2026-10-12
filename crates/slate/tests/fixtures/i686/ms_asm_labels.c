/* { dg-options "-fms-extensions" } */
#include <stdio.h>

static int loop_label(int n) {
  int acc = 0;
  __asm {
    mov ecx, n
    xor eax, eax
  top:
    add eax, ecx
    dec ecx
    jnz top
    mov acc, eax
  }
  return acc;
}

static int forward(int v) {
  int out = 0;
  __asm {
    mov eax, v
    test eax, eax
    jz done
    mov eax, 100
  done:
    mov out, eax
  }
  return out;
}

static int two_blocks(int n) {
  int out = 0;
  __asm {
    mov ecx, n
    xor eax, eax
  again:
    inc eax
    dec ecx
    jnz again
    mov out, eax
  }
  __asm {
    mov ecx, 3
  again:
    inc out
    dec ecx
    jnz again
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", loop_label(4), forward(0), forward(5), two_blocks(2));
  return 0;
}
