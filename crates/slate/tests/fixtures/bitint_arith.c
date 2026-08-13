#include <stdio.h>

void abort(void);

struct W {
  int tag;
  _BitInt(128) s;
  unsigned _BitInt(128) u;
};

static _BitInt(128) add128(_BitInt(128) a, _BitInt(128) b) { return a + b; }

static unsigned _BitInt(128)
    mul128(unsigned _BitInt(128) a, unsigned _BitInt(128) b) {
  return a * b;
}

static int switch_wide_case_u128(unsigned _BitInt(128) x) {
  switch (x) {
  case 200000000000000000000000000000000000000uwb:
    return 1;
  default:
    return 0;
  }
}

static int switch_wide_selector(_BitInt(256) x) {
  switch (x) {
  case 1:
  case 2:
    return 10;
  case -987654321098765432109876543210987654321098765432109876543210wb:
    return 20;
  case 300 ... 400:
    return 30;
  default:
    return 0;
  }
}

static int switch_wide_selector_unsigned(unsigned _BitInt(256) x) {
  switch (x) {
  case 123456789012345678901234567890123456789012345678901234567890uwb:
    return 1;
  case 0:
    return 2;
  default:
    return 3;
  }
}

static int switch_wide_fallthrough(_BitInt(256) x) {
  int acc = 0;
  switch (x) {
  case 1:
    acc += 100;
  case 2:
    acc += 1;
    break;
  case 3:
    acc = 999;
    break;
  default:
    acc = -1;
  }
  return acc;
}

static _BitInt(256) add256(_BitInt(256) a, _BitInt(256) b) { return a + b; }

static unsigned _BitInt(256)
    mul256(unsigned _BitInt(256) a, unsigned _BitInt(256) b) {
  return a * b;
}

// Odd, non-64-aligned widths: exercise BITS that don't line up with a
// native Rust integer or a whole number of 64-bit limbs. Values stay well
// inside each width's representable range, so wraparound never fires; the
// invariants below (undoing +/- and double negation) hold for any width
// under two's-complement arithmetic, so they need no per-width hand
// computation.
static void check_bitint3(void) {
  _BitInt(3) x   = 1;
  _BitInt(3) y   = -2;
  _BitInt(3) sum = x + y;
  if (sum - y != x)
    abort();
  if (-(-x) != x)
    abort();
  if (!(y < x))
    abort();

  unsigned _BitInt(3) ux   = 5;
  unsigned _BitInt(3) uy   = 2;
  unsigned _BitInt(3) usum = ux + uy;
  if (usum - uy != ux)
    abort();
}

static void check_bitint129(void) {
  _BitInt(129) x  = 123456789;
  _BitInt(129) y  = -987654321;
  _BitInt(129) s2 = x + y;
  if (s2 - y != x)
    abort();
  if (-(-x) != x)
    abort();
  if (!(y < x))
    abort();

  unsigned _BitInt(129) ux   = 123456789;
  unsigned _BitInt(129) uy   = 987654321;
  unsigned _BitInt(129) usum = ux + uy;
  if (usum - uy != ux)
    abort();
}

static void check_bitint512(void) {
  _BitInt(512) x  = 123456789012345LL;
  _BitInt(512) y  = -987654321098765LL;
  _BitInt(512) s3 = x + y;
  if (s3 - y != x)
    abort();
  if (-(-x) != x)
    abort();
  if (!(y < x))
    abort();

  unsigned _BitInt(512) ux   = 123456789012345ULL;
  unsigned _BitInt(512) uy   = 987654321098765ULL;
  unsigned _BitInt(512) usum = ux + uy;
  if (usum - uy != ux)
    abort();
}

// Decimal literals whose magnitude exceeds u128::MAX (2^128) but still fits
// the target _BitInt(N) width, exercising the arbitrary-width decimal-string
// parse path instead of the i128/u128 fast paths.
static void check_bitint_wide_literal(void) {
  _BitInt(256) x256 =
      123456789012345678901234567890123456789012345678901234567890wb;
  _BitInt(256) y256 =
      -123456789012345678901234567890123456789012345678901234567890wb;
  if (x256 + y256 != 0)
    abort();
  if (-x256 != y256)
    abort();
  if (!(y256 < x256))
    abort();

  unsigned _BitInt(256) ux256 =
      123456789012345678901234567890123456789012345678901234567890uwb;
  unsigned _BitInt(256) uy256   = 300uwb;
  unsigned _BitInt(256) usum256 = ux256 + uy256;
  if (usum256 - uy256 != ux256)
    abort();

  _BitInt(512) x512 =
      123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890wb;
  _BitInt(512) y512 =
      -123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890wb;
  if (x512 + y512 != 0)
    abort();
  if (-x512 != y512)
    abort();
  if (!(y512 < x512))
    abort();

  unsigned _BitInt(512) ux512 =
      123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890uwb;
  unsigned _BitInt(512) uy512   = 512uwb;
  unsigned _BitInt(512) usum512 = ux512 + uy512;
  if (usum512 - uy512 != ux512)
    abort();
}

int main(void) {
  _BitInt(128) x128   = -12345;
  _BitInt(128) y128   = 100;
  _BitInt(128) sum128 = add128(x128, y128);
  printf("%lld\n", (long long)sum128);

  unsigned _BitInt(128) p128    = 1000000000000ULL;
  unsigned _BitInt(128) q128    = 3;
  unsigned _BitInt(128) prod128 = mul128(p128, q128);
  printf("%llu\n", (unsigned long long)prod128);

  struct W w;
  w.tag  = 1;
  w.s    = -5;
  w.u    = 12345678901234567890ULL;
  w.s   += 10;
  w.u   *= 2;
  printf("%zu\n", sizeof(struct W));
  printf("%lld\n", (long long)w.s);
  printf("%llu\n", (unsigned long long)w.u);

  _BitInt(256) x256   = 12345;
  _BitInt(256) y256   = -300;
  _BitInt(256) sum256 = add256(x256, y256);
  if (sum256 != 12045)
    abort();

  unsigned _BitInt(256) p256    = 1000000000000ULL;
  unsigned _BitInt(256) q256    = 3;
  unsigned _BitInt(256) prod256 = mul256(p256, q256);
  if (prod256 != 3000000000000ULL)
    abort();

  _BitInt(256) neg256 = -x256;
  if (neg256 != -12345)
    abort();

  _BitInt(256) diff256 = x256 - y256;
  if (diff256 != 12645)
    abort();

  if (!(y256 < x256))
    abort();
  if (!(x256 > y256))
    abort();

  check_bitint3();
  check_bitint129();
  check_bitint512();
  check_bitint_wide_literal();

  if (switch_wide_case_u128(200000000000000000000000000000000000000uwb) != 1)
    abort();
  if (switch_wide_case_u128(1) != 0)
    abort();

  if (switch_wide_selector(1) != 10)
    abort();
  if (switch_wide_selector(2) != 10)
    abort();
  if (switch_wide_selector(
          -987654321098765432109876543210987654321098765432109876543210wb) !=
      20)
    abort();
  if (switch_wide_selector(350) != 30)
    abort();
  if (switch_wide_selector(300) != 30)
    abort();
  if (switch_wide_selector(400) != 30)
    abort();
  if (switch_wide_selector(999) != 0)
    abort();

  if (switch_wide_selector_unsigned(
          123456789012345678901234567890123456789012345678901234567890uwb) != 1)
    abort();
  if (switch_wide_selector_unsigned(0) != 2)
    abort();
  if (switch_wide_selector_unsigned(42) != 3)
    abort();

  if (switch_wide_fallthrough(1) != 101)
    abort();
  if (switch_wide_fallthrough(2) != 1)
    abort();
  if (switch_wide_fallthrough(3) != 999)
    abort();
  if (switch_wide_fallthrough(4) != -1)
    abort();

  return 0;
}
