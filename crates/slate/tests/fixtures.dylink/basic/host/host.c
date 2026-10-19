#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"

typedef int (*add_fn)(int, int);
typedef long long (*mul64_fn)(long long, long long);
typedef unsigned char (*not8_fn)(unsigned char);
typedef double (*hypot2_fn)(double, double);

int main(void) {
  void *missing = dl_open("no_such_library_dep");
  printf("missing library: %s\n", missing ? "loaded" : "failed");

  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  add_fn add = (add_fn)dl_sym(lib, "plug_add");
  mul64_fn mul64 = (mul64_fn)dl_sym(lib, "plug_mul64");
  not8_fn not8 = (not8_fn)dl_sym(lib, "plug_not8");
  hypot2_fn hypot2 = (hypot2_fn)dl_sym(lib, "plug_hypot2");
  if (!add || !mul64 || !not8 || !hypot2) {
    printf("resolve failed\n");
    return 2;
  }
  printf("plug_add(3, 4) = %d\n", add(3, 4));
  printf("plug_add(-7, 2147483647) = %d\n", add(-7, 2147483647));
  printf("plug_mul64 = %lld\n", mul64(3000000000LL, -7LL));
  printf("plug_not8(0x0f) = %u\n", (unsigned)not8(0x0f));
  printf("plug_hypot2 = %.3f\n", hypot2(1.5, 2.0));
  printf("missing symbol: %s\n", dl_sym(lib, "plug_nope") ? "found" : "absent");
  printf("internal symbol: %s\n", dl_sym(lib, "plug_internal") ? "found" : "absent");
  int (*use_internal)(int) = (int (*)(int))dl_sym(lib, "plug_use_internal");
  printf("use_internal(4) = %d\n", use_internal ? use_internal(4) : -1);
  printf("close: %s\n", dl_close(lib) ? "ok" : "failed");
  return 0;
}
