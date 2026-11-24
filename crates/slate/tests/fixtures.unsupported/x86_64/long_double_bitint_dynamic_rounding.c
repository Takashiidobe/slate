// dg-options "-frounding-math"
#include <fenv.h>
typedef unsigned _BitInt(65) Wide;
static long double convert(Wide value) { return (long double)value; }
int main(void) {
  volatile Wide value = ((Wide)1 << 64) + 1;
  if (fesetround(FE_UPWARD)) return 4;
  long double up = convert(value);
  if (fesetround(FE_DOWNWARD)) return 4;
  long double down = convert(value);
  return (up != 0x1.0000000000000002p64L) + 2 * (down != 0x1p64L);
}
