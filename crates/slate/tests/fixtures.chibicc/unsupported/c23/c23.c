#include "test.h"

[[noreturn]] void die(void) {
  for (;;) {
  }
}
[[deprecated("use new_func")]] int old_func(void) { return 9; }
[[deprecated]] int                 old_func2(void) { return 10; }
int                                add_unnamed(int, int) { return 5; }

int main(void) {
  bool t = true;
  bool f = false;
  ASSERT(1, t);
  ASSERT(0, f);
  int *p = nullptr;
  ASSERT(1, p == nullptr);
  ASSERT(0, p != nullptr);

#define WARN_MSG "c23 warning"
#warning WARN_MSG

  int bin = 0b101010;
  ASSERT(42, bin);
  int dec = 1'000'000;
  int hex = 0xFF'FF;
  ASSERT(1000000, dec);
  ASSERT(65535, hex);
  int          base      = 7;
  typeof(base) t1        = base + 1;
  typeof_unqual(base) t2 = t1;
  ASSERT(8, t1);
  ASSERT(8, t2);
  ASSERT(9, old_func());
  ASSERT(10, old_func2());
  int fall = 0;
  switch (base) {
  case 7:
    fall = 1;
    [[fallthrough]];
  case 8:
    fall += 2;
    break;
  default:
    fall = -1;
  }
  ASSERT(3, fall);
  int label_check = 0;
  {
    goto end;
    label_check = 1;
  end:
  }
  ASSERT(0, label_check);
  unsigned char embed_data[] = {
#embed "embed.bin"
  };
  ASSERT(3, (int)sizeof(embed_data));
  ASSERT(1, embed_data[0]);
  ASSERT(2, embed_data[1]);
  ASSERT(255, embed_data[2]);

#define FOO 1
#define BAR 1
  int x = 0;
#if 0
  x = 1;
#elifdef FOO
  x = 2;
#elifndef BAR
  x = 3;
#else
  x = 4;
#endif
  ASSERT(2, x);

#undef BAR
#ifdef FOO
  x = 5;
#elifdef FOO
  x = 6;
#elifndef BAR
  x = 7;
#else
  x = 8;
#endif
  ASSERT(5, x);

#undef FOO
#if 0
  x = 9;
#elifdef FOO
  x = 10;
#elifndef BAR
  x = 11;
#else
  x = 12;
#endif
  ASSERT(11, x);
  ASSERT(5, add_unnamed(2, 3));
  enum Small : unsigned char { SA = 1, SB = 2 };
  enum Small s = SA;
  ASSERT(1, s);
  ASSERT(1, (int)sizeof(s));
  constexpr int size = 4;
  int           arr[size];
  ASSERT(4, (int)(sizeof(arr) / sizeof(arr[0])));

  // _BitInt tests
  _BitInt(8) b8 = 42;
  ASSERT(42, (int)b8);
  _BitInt(16) b16 = 1000;
  ASSERT(1000, (int)b16);
  _BitInt(32) b32 = 100000;
  ASSERT(100000, (int)b32);
  _BitInt(64) b64 = 123456789;
  ASSERT(123456789, (int)b64);

  // Unsigned _BitInt
  unsigned _BitInt(8) ub8 = 200;
  ASSERT(200, (int)ub8);
  unsigned _BitInt(32) ub32 = 3000000000u;
  ASSERT(-1294967296, (int)ub32); // Wraps when cast to signed int

  // Arithmetic
  _BitInt(32) a = 100;
  _BitInt(32) b = 50;
  ASSERT(150, (int)(a + b));
  ASSERT(50, (int)(a - b));
  ASSERT(5000, (int)(a * b));
  ASSERT(2, (int)(a / b));
  ASSERT(0, (int)(a % b));

  // sizeof
  ASSERT(1, (int)sizeof(_BitInt(8)));
  ASSERT(2, (int)sizeof(_BitInt(16)));
  ASSERT(4, (int)sizeof(_BitInt(32)));
  ASSERT(8, (int)sizeof(_BitInt(64)));

  // 128-bit _BitInt tests (Tier 2)
  ASSERT(16, (int)sizeof(_BitInt(128)));
  ASSERT(16, (int)sizeof(_BitInt(65)));

  // Basic 128-bit arithmetic with small values
  _BitInt(128) c128 = 100;
  _BitInt(128) d128 = 50;
  ASSERT(150, (int)(c128 + d128));
  ASSERT(50, (int)(c128 - d128));
  ASSERT(5000, (int)(c128 * d128));
  ASSERT(2, (int)(c128 / d128));
  ASSERT(0, (int)(c128 % d128));

  // Bitwise operations on 128-bit
  _BitInt(128) e128 = 0xFF;
  _BitInt(128) f128 = 0x0F;
  ASSERT(0x0F, (int)(e128 & f128));
  ASSERT(0xFF, (int)(e128 | f128));
  ASSERT(0xF0, (int)(e128 ^ f128));

  // Comparisons on 128-bit
  ASSERT(1, c128 > d128);
  ASSERT(0, c128 < d128);
  ASSERT(1, c128 != d128);
  ASSERT(0, c128 == d128);
  _BitInt(128) g128 = 100;
  ASSERT(1, c128 == g128);

  // 256-bit _BitInt tests (Tier 3)
  ASSERT(32, (int)sizeof(_BitInt(256)));
  ASSERT(24, (int)sizeof(_BitInt(129)));

  // Basic 256-bit arithmetic with small values
  _BitInt(256) a256 = 100;
  _BitInt(256) b256 = 50;
  ASSERT(150, (int)(a256 + b256));
  ASSERT(50, (int)(a256 - b256));

  // Bitwise operations on 256-bit
  _BitInt(256) c256 = 0xFF;
  _BitInt(256) d256 = 0x0F;
  ASSERT(0x0F, (int)(c256 & d256));
  ASSERT(0xFF, (int)(c256 | d256));
  ASSERT(0xF0, (int)(c256 ^ d256));

  // Comparisons on 256-bit
  ASSERT(1, a256 > b256);
  ASSERT(0, a256 < b256);
  ASSERT(1, a256 != b256);
  ASSERT(0, a256 == b256);
  _BitInt(256) e256 = 100;
  ASSERT(1, a256 == e256);

  return 0;
}
