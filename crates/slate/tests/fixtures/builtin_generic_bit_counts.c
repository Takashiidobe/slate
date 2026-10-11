#include <stdio.h>

static volatile unsigned char           uc   = 0x10;
static volatile unsigned short          us   = 0x0180;
static volatile unsigned int            ui   = 0x00f0u;
static volatile unsigned long           ul   = 0x8000ul;
static volatile unsigned long long      ull  = 0x0000000300000000ull;
static volatile unsigned __int128       u128 = (unsigned __int128)1 << 100;
static volatile unsigned int            zero = 0;

static int side_effects;

static int fallback(int value) {
  side_effects++;
  return value;
}

int main(void) {
  printf("uc %d %d %d\n", __builtin_clzg(uc), __builtin_ctzg(uc),
         __builtin_popcountg(uc));
  printf("us %d %d %d\n", __builtin_clzg(us), __builtin_ctzg(us),
         __builtin_popcountg(us));
  printf("ui %d %d %d\n", __builtin_clzg(ui), __builtin_ctzg(ui),
         __builtin_popcountg(ui));
  printf("ul %d %d %d\n", __builtin_clzg(ul), __builtin_ctzg(ul),
         __builtin_popcountg(ul));
  printf("ull %d %d %d\n", __builtin_clzg(ull), __builtin_ctzg(ull),
         __builtin_popcountg(ull));
  printf("u128 %d %d %d\n", __builtin_clzg(u128), __builtin_ctzg(u128),
         __builtin_popcountg(u128));
  printf("zero %d %d %d\n", __builtin_clzg(zero, -1), __builtin_ctzg(zero, 99),
         __builtin_popcountg(zero));
  printf("nonzero %d %d\n", __builtin_clzg(ui, fallback(-1)),
         __builtin_ctzg(ui, fallback(99)));
  printf("side effects %d\n", side_effects);
  return 0;
}
