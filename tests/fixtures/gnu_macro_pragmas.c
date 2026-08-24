// LOWERING-DAG: static mut macro_inner: i32 = 11;
// LOWERING-DAG: static mut macro_outer: i32 = 7;
// LOWERING-NOT: poisoned_but_unused

#include <stdio.h>

#define GNU_MACRO_VALUE 7
#pragma push_macro("GNU_MACRO_VALUE")
#undef GNU_MACRO_VALUE
#define GNU_MACRO_VALUE 11
static int macro_inner = GNU_MACRO_VALUE;
#pragma pop_macro("GNU_MACRO_VALUE")
static int macro_outer = GNU_MACRO_VALUE;

#pragma GCC poison poisoned_but_unused

int main(void) {
  printf("%d %d\n", macro_inner, macro_outer);
  return 0;
}
