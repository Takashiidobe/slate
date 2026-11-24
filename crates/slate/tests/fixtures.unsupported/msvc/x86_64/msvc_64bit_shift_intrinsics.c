// { dg-options "/Zc:preprocessor" }
#include <intrin.h>

#pragma intrinsic(__ll_lshift, __ll_rshift, __ull_rshift)

static int check(int count) {
  int left_failed = __ll_lshift(1, count) != 2;
  int signed_failed = __ll_rshift(-16, count) != -8;
  int unsigned_failed = __ull_rshift(0x8000000000000000ULL, count) !=
                        0x4000000000000000ULL;
  return left_failed + 2 * signed_failed + 4 * unsigned_failed;
}

int main(void) {
  return check(65);
}
