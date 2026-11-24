// { dg-options "/Zc:preprocessor" }
#include <intrin.h>

#pragma intrinsic(_BitScanForward, _BitScanReverse)

static int check32(unsigned long mask) {
  unsigned long index;
  if (!_BitScanForward(&index, mask) || index != 4)
    return 1;
  if (!_BitScanReverse(&index, mask) || index != 31)
    return 2;
  return _BitScanForward(&index, 0) || _BitScanReverse(&index, 0);
}

#if defined(_M_X64) || defined(_M_ARM64)
#pragma intrinsic(_BitScanForward64, _BitScanReverse64)

static int check64(unsigned __int64 mask) {
  unsigned long index;
  if (!_BitScanForward64(&index, mask) || index != 32)
    return 1;
  if (!_BitScanReverse64(&index, mask) || index != 63)
    return 2;
  return _BitScanForward64(&index, 0) || _BitScanReverse64(&index, 0);
}
#endif

int main(void) {
  int failed = check32(0x80000010UL) != 0;
#if defined(_M_X64) || defined(_M_ARM64)
  failed |= 2 * (check64(0x8000000100000000ULL) != 0);
#endif
  return failed;
}
