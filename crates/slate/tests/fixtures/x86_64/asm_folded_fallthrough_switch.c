#include <stdio.h>

extern void __bad_size(void);

#define STEP(x) x = x * 3 + 1; x ^= x >> 3;
#define TAIL(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x) STEP(x)

#define WIDEN(x, p)                                                            \
  do {                                                                         \
    switch (sizeof(*(p))) {                                                    \
    case 1:                                                                    \
      __asm__("movzbl %1, %k0" : "=r"(x) : "q"(*(p)));                         \
    case 2:                                                                    \
      TAIL(x)                                                                  \
      break;                                                                   \
    case 4:                                                                    \
      __asm__("movl %1, %k0" : "=r"(x) : "r"(*(p)));                           \
    case 8:                                                                    \
      TAIL(x)                                                                  \
      break;                                                                   \
    default:                                                                   \
      __bad_size();                                                            \
    }                                                                          \
  } while (0)

static unsigned long widen_u8(unsigned char *p) { unsigned long x = 0; WIDEN(x, p); return x; }
static unsigned long widen_u32(unsigned int *p) { unsigned long x = 0; WIDEN(x, p); return x; }
static unsigned long widen_u64(unsigned long *p) { unsigned long x = 5; WIDEN(x, p); return x; }

int main(void) {
  unsigned char a = 3;
  unsigned int b = 4;
  unsigned long c = 6;
  printf("%lu %lu %lu\n", widen_u8(&a), widen_u32(&b), widen_u64(&c));
  return 0;
}
