#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER >= 1700 && defined(_M_ARM)
#pragma optimize("", off)
#endif

int msvc_arm_guarded_pragma(int x) { return x + 41; }

int main(void) {
  printf("%d\n", msvc_arm_guarded_pragma(1));
  return 0;
}
