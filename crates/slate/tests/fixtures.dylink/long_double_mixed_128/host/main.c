#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"
#include "../abi.h"

static long double add_quad_callback(__float128 a, long double b) {
  return (long double)a + b;
}

static long double add_unsigned_callback(unsigned __int128 a, long double b) {
  return (long double)(a >> 96) + b;
}

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib)
    return 1;
  quad_fn add_quad = (quad_fn)dl_sym(lib, "add_quad");
  signed_fn add_signed = (signed_fn)dl_sym(lib, "add_signed");
  check_quad_fn check_quad = (check_quad_fn)dl_sym(lib, "check_quad");
  check_unsigned_fn check_unsigned =
      (check_unsigned_fn)dl_sym(lib, "check_unsigned");
  if (!add_quad || !add_signed || !check_quad || !check_unsigned)
    return 2;

  printf("quad call=%d\n",
         add_quad((__float128)0x1p-63L, 1.0L) == 0x1.0000000000000002p0L);
  printf("signed call=%d\n", add_signed((__int128)2 << 96, 1.0L) == 3.0L);
  printf("quad callback=%d\n", check_quad(add_quad_callback));
  printf("unsigned callback=%d\n", check_unsigned(add_unsigned_callback));
  return dl_close(lib) ? 0 : 3;
}
