#include "dylink.h"
#include "../abi.h"

EXPORT long double add_quad(__float128 a, long double b) {
  return (long double)a + b;
}

EXPORT long double add_signed(__int128 a, long double b) {
  return (long double)(a >> 96) + b;
}

EXPORT int check_quad(quad_fn fn) {
  return fn((__float128)0x1p-63L, 1.0L) == 0x1.0000000000000002p0L;
}

EXPORT int check_unsigned(unsigned_fn fn) {
  return fn((unsigned __int128)2 << 96, 1.0L) == 3.0L;
}
