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
  w.tag = 1;
  w.s   = -5;
  w.u   = 12345678901234567890ULL;
  w.s  += 10;
  w.u  *= 2;
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

  return 0;
}
