#include <stdio.h>

typedef _BitInt(128) big;

// @check-fn-begin
int classify_u64(unsigned long long v) {
  switch (v) {
  case 18446744073709551610ULL:
  case 18446744073709551611ULL:
  case 18446744073709551612ULL:
    return 1;
  case 18446744073709551615ULL:
    return 2;
  default:
    return 0;
  }
}
// @check-fn-end

// @check-fn-begin
int classify_bitint(int seed) {
  big v = (big)170141183460469231731687303715884105720wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105720wb:
  case 170141183460469231731687303715884105721wb:
  case 170141183460469231731687303715884105722wb:
    return 1;
  case 170141183460469231731687303715884105727wb:
    return 2;
  default:
    return 0;
  }
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d %d\n", classify_u64(18446744073709551611ULL),
         classify_u64(18446744073709551615ULL), classify_u64(3),
         classify_bitint(1), classify_bitint(7), classify_bitint(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn classify_u64(mut v: u64) -> i32 {
// CHECK-NEXT:     match v {
// CHECK-NEXT:         18446744073709551610..=18446744073709551612 => {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         18446744073709551615 => {
// CHECK-NEXT:             return 2 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn classify_bitint(mut seed: i32) -> i32 {
// CHECK-NEXT:     let mut v: i128 = (170141183460469231731687303715884105720 as i128) + (seed as i128);
// CHECK-NEXT:     match v {
// CHECK-NEXT:         170141183460469231731687303715884105720..=170141183460469231731687303715884105722 => {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         170141183460469231731687303715884105727 => {
// CHECK-NEXT:             return 2 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
