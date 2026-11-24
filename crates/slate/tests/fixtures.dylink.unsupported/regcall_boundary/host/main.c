#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"
#include "../abi.h"

static long __attribute__((regcall))
sum_callback(long a, long b, long c, long d, long e, long f) {
  return a + 2 * b + 4 * c + 8 * d + 16 * e + 32 * f;
}

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib)
    return 1;
  sum_fn weighted_sum = (sum_fn)dl_sym(lib, "weighted_sum");
  check_fn check_sum = (check_fn)dl_sym(lib, "check_sum");
  if (!weighted_sum || !check_sum)
    return 2;

  printf("regcall call=%d\n", weighted_sum(1, 2, 3, 4, 5, 6) == 321);
  printf("regcall callback=%d\n", check_sum(sum_callback));
  return dl_close(lib) ? 0 : 3;
}
