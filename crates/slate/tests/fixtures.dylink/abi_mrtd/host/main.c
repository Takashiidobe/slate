#include <stdio.h>
#include <dlfcn.h>
#include "dylink_path.h"
#include "../abi_mrtd.h"

typedef void *(__attribute__((cdecl)) *open_fn)(const char *, int);
typedef void *(__attribute__((cdecl)) *sym_fn)(void *, const char *);
typedef int(__attribute__((cdecl)) *close_fn)(void *);

static int host_mul(int a, int b) { return a * b % 1009; }

static int host_xor(int a, int b) { return (a ^ b) & 0xff; }

int main(void) {
  open_fn open = (open_fn)dlopen;
  sym_fn sym = (sym_fn)dlsym;
  close_fn close = (close_fn)dlclose;
  void *lib = open(DEP_PATH, RTLD_NOW | RTLD_LOCAL);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  binary_fn sub = (binary_fn)sym(lib, "mrtd_sub");
  wide_fn wide = (wide_fn)sym(lib, "mrtd_wide");
  apply_fn apply = (apply_fn)sym(lib, "mrtd_apply");
  sum_fn sum = (sum_fn)sym(lib, "mrtd_sum");
  make_fn make = (make_fn)sym(lib, "mrtd_make");
  mix_fn mix = (mix_fn)sym(lib, "mrtd_mix");
  if (!sub || !wide || !apply || !sum || !make || !mix) {
    printf("resolve failed\n");
    return 2;
  }
  int acc = 0;
  for (int i = 0; i < 1000; i++)
    acc += sub(i, 3);
  printf("sub loop = %d\n", acc);
  printf("wide = %lld\n", wide(3000000000LL, -7, -2));
  printf("apply mul = %d\n", apply(host_mul, 5, 7));
  printf("apply xor = %d\n", apply(host_xor, 1, 0x5a));
  printf("sum = %d\n", sum(4, 10, 20, 30, 40));
  struct pair p = make(14, 9);
  printf("make = %d %d\n", p.a, p.b);
  printf("mix = %.3f\n", mix(1.5f, 2.25, -4));
  printf("close = %d\n", close(lib));
  return 0;
}
