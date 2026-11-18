#include "dylink.h"
#include "../abi_reg_struct_return.h"

EXPORT struct one rsr_one(int v) { struct one r = {(char)(v + 1)}; return r; }
EXPORT struct two rsr_two(int v) { struct two r = {(short)(v * 300)}; return r; }
EXPORT struct four rsr_four(int v) { struct four r = {(short)-v, (char)(v & 0x7f)}; return r; }
EXPORT struct eight rsr_eight(int v) { struct eight r = {v, -v * 7}; return r; }
EXPORT struct mixed rsr_mixed(int v) { struct mixed r = {v + 2, v * 0.5f}; return r; }
EXPORT struct three rsr_three(int v) { struct three r = {{(char)v, (char)(v + 1), (char)(v + 2)}}; return r; }
EXPORT struct twelve rsr_twelve(int v) { struct twelve r = {v, v * 2, v * 3}; return r; }
EXPORT struct single_float rsr_single_float(int v) { struct single_float r = {v * 1.25f}; return r; }
EXPORT struct single_double rsr_single_double(int v) { struct single_double r = {v / 8.0}; return r; }
EXPORT union word rsr_word(int v) { union word r = {v ^ 0x1234}; return r; }
EXPORT _Complex float rsr_complex(int v) { return v + 2.0f * v * 1.0fi; }

EXPORT int rsr_sum_eight(eight_fn f, int n) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    struct eight e = f(i);
    total += e.a * 3 + e.b;
  }
  return total;
}

EXPORT double rsr_sum_double(single_double_fn f, int n) {
  double total = 0;
  for (int i = 0; i < n; i++)
    total += f(i).d;
  return total;
}

EXPORT int rsr_sum_three(three_fn f, int n) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    struct three t = f(i);
    total += t.a[0] + t.a[1] * 2 + t.a[2] * 3;
  }
  return total;
}
