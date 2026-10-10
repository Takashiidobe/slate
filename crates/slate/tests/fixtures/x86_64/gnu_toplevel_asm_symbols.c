#include <stdio.h>

static int counter = 40;

__attribute__((used)) static int twice(int value) { return value * 2; }

static int add(int left, int right) { return left + right; }

__asm__(".text\n"
        ".globl toplevel_bump\n"
        ".type toplevel_bump,@function\n"
        "toplevel_bump:\n"
        "  incl counter(%rip) # {braces}\n"
        "  movl counter(%rip), %eax\n"
        "  add $1, %eax\n"
        "  ret\n"
        ".size toplevel_bump,.-toplevel_bump\n");

__asm__(".text\n"
        ".globl toplevel_twice\n"
        ".type toplevel_twice,@function\n"
        "toplevel_twice:\n"
        "  jmp twice\n"
        ".size toplevel_twice,.-toplevel_twice\n");

extern int toplevel_bump(void);
extern int toplevel_twice(int value);

int main(void) {
  int bumped = toplevel_bump();
  printf("%d %d %d %d\n", bumped, counter, toplevel_twice(21), add(2, 3));
  return 0;
}
