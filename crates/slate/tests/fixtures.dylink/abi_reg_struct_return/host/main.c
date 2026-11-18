#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"
#include "../abi_reg_struct_return.h"

static struct eight host_eight(int v) { struct eight r = {v * 5, v - 9}; return r; }
static struct single_double host_double(int v) { struct single_double r = {v * 0.75}; return r; }
static struct three host_three(int v) { struct three r = {{(char)(v * 2), (char)(v + 7), (char)-v}}; return r; }

#define RESOLVE(type, name)                      \
  type name = (type)dl_sym(lib, "rsr_" #name); \
  if (!name) {                                   \
    printf("resolve %s failed\n", #name);        \
    return 2;                                    \
  }

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  RESOLVE(one_fn, one)
  RESOLVE(two_fn, two)
  RESOLVE(four_fn, four)
  RESOLVE(eight_fn, eight)
  RESOLVE(mixed_fn, mixed)
  RESOLVE(three_fn, three)
  RESOLVE(twelve_fn, twelve)
  RESOLVE(single_float_fn, single_float)
  RESOLVE(single_double_fn, single_double)
  RESOLVE(word_fn, word)
  RESOLVE(complex_fn, complex)
  RESOLVE(sum_eight_fn, sum_eight)
  RESOLVE(sum_double_fn, sum_double)
  RESOLVE(sum_three_fn, sum_three)
  printf("one = %d\n", one(41).a);
  printf("two = %d\n", two(7).a);
  struct four f = four(99);
  printf("four = %d %d\n", f.a, f.b);
  struct eight e = eight(12);
  printf("eight = %d %d\n", e.a, e.b);
  struct mixed m = mixed(9);
  printf("mixed = %d %.2f\n", m.a, m.b);
  struct three t = three(20);
  printf("three = %d %d %d\n", t.a[0], t.a[1], t.a[2]);
  struct twelve w = twelve(5);
  printf("twelve = %d %d %d\n", w.a, w.b, w.c);
  printf("single_float = %.3f\n", single_float(6).f);
  printf("single_double = %.4f\n", single_double(11).d);
  printf("word = %#x\n", word(0x55).i);
  _Complex float c = complex(3);
  printf("complex = %.1f %.1f\n", __real__ c, __imag__ c);
  printf("sum_eight = %d\n", sum_eight(host_eight, 50));
  printf("sum_double = %.3f\n", sum_double(host_double, 40));
  printf("sum_three = %d\n", sum_three(host_three, 30));
  printf("close: %s\n", dl_close(lib) ? "ok" : "failed");
  return 0;
}
