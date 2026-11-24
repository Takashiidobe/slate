#include <limits.h>

static int signed_add(int a, int b) {
  int result;
  return !__builtin_sadd_overflow(a, b, &result) || result != INT_MIN;
}

static int signed_sub(long a, long b) {
  long result;
  return !__builtin_ssubl_overflow(a, b, &result) || result != LONG_MAX;
}

static int signed_mul(long long a, long long b) {
  long long result;
  return !__builtin_smulll_overflow(a, b, &result) || result != LLONG_MIN;
}

static int unsigned_add(unsigned a, unsigned b) {
  unsigned result;
  return !__builtin_uadd_overflow(a, b, &result) || result != 0;
}

static int unsigned_sub(unsigned long a, unsigned long b) {
  unsigned long result;
  return !__builtin_usubl_overflow(a, b, &result) || result != ULONG_MAX;
}

int main(void) {
  return signed_add(INT_MAX, 1) + 2 * signed_sub(LONG_MIN, 1) +
         4 * signed_mul(LLONG_MIN, -1) + 8 * unsigned_add(UINT_MAX, 1) +
         16 * unsigned_sub(0, 1);
}
