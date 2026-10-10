#include <stdio.h>

typedef struct { long a, b, c; } big_t;

static unsigned __int128 pair_out(void) {
  unsigned __int128 r;
  __asm__("movq $7, %%rax\n\tmovq $9, %%rdx # %0" : "=A"(r));
  return r;
}

static unsigned __int128 pair_inout(unsigned __int128 x) {
  __asm__("addq $1, %q0\n\tadcq $0, %%rdx" : "+A"(x));
  return x;
}

static unsigned long pair_in_high(unsigned __int128 x) {
  unsigned long hi;
  __asm__("movq %%rdx, %0" : "=r"(hi) : "A"(x));
  return hi;
}

static unsigned long single_a(unsigned long x) {
  unsigned long r;
  __asm__("leaq 3(%1), %0" : "=A"(r) : "l"(x));
  return r;
}

static int any_operands(int x, big_t *big) {
  __asm__ volatile("" : : "X"(x), "X"(5), "X"(*big));
  int r;
  __asm__("movl $42, %0" : "=X"(r));
  return r + x - 40;
}

int main(void) {
  unsigned __int128 a = pair_out();
  unsigned __int128 b = pair_inout(((unsigned __int128)4 << 64) | ~0UL);
  big_t big = {1, 2, 3};
  printf("%lu %lu\n", (unsigned long)(a >> 64), (unsigned long)a);
  printf("%lu %lu\n", (unsigned long)(b >> 64), (unsigned long)b);
  printf("%lu %lu %d\n", pair_in_high(((unsigned __int128)11 << 64) | 12),
         single_a(39), any_operands(40, &big));
  return 0;
}
