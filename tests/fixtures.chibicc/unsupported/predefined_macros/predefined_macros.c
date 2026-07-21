#include "test.h"

int main() {
  // Required C11 macros
  ASSERT(1, __STDC__);
  ASSERT(1, __STDC_HOSTED__);
  ASSERT(201710, __STDC_VERSION__); // C17

  // Optional feature support
  ASSERT(1, __STDC_UTF_16__);
  ASSERT(1, __STDC_UTF_32__);
  ASSERT(1, __STDC_IEC_559__); // IEEE 754 floating point

  // C11 char16_t and char32_t underlying type macros
  ASSERT(2, sizeof(__CHAR16_TYPE__));
  ASSERT(4, sizeof(__CHAR32_TYPE__));

  // These should NOT be defined (we support atomics, VLAs, and threads)
#ifdef __STDC_NO_ATOMICS__
  ASSERT(0, 1); // fail if defined
#endif
#ifdef __STDC_NO_VLA__
  ASSERT(0, 1); // fail if defined
#endif
#ifdef __STDC_NO_THREADS__
  ASSERT(0, 1); // fail if defined
#endif

  printf("OK\n");
  return 0;
}
