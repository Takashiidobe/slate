/* { dg-options "-fms-extensions" } */
#include <stdio.h>
int sink;
static void bump(void) { sink += 100; }
static void call_fn(void) {
  __asm {
    call bump
  }
}
static int clobbers(void) {
  int r = 5;
  __asm {
    mov ecx, 3
    mov edx, 4
    add ecx, edx
  }
  return r + 1;
}
static int stack_balance(void) {
  int r = 9;
  __asm {
    push 7
    push 8
    pop ecx
    pop edx
    add ecx, edx
  }
  return r;
}
int main(void) {
  call_fn();
  printf("%d %d\n", sink, clobbers());
  printf("%d\n", stack_balance());
  return 0;
}
