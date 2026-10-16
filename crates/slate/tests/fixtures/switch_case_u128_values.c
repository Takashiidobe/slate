#include <stdio.h>

typedef unsigned _BitInt(128) u128;

// @check-fn-begin
int classify(int seed) {
  u128 v = (u128)340282366920938463463374607431768211440uwb + (u128)seed;
  switch (v) {
  case 340282366920938463463374607431768211441uwb:
  case 340282366920938463463374607431768211443uwb:
    return 1;
  case 340282366920938463463374607431768211450uwb ... 340282366920938463463374607431768211453uwb:
    return 2;
  default:
    return 0;
  }
}
// @check-fn-end

// @check-fn-begin
int classify_run(int seed) {
  u128 v = (u128)340282366920938463463374607431768211440uwb + (u128)seed;
  switch (v) {
  case 340282366920938463463374607431768211441uwb:
  case 340282366920938463463374607431768211442uwb:
  case 340282366920938463463374607431768211443uwb:
    return 1;
  case 340282366920938463463374607431768211450uwb:
    return 2;
  default:
    return 0;
  }
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify(1), classify(3), classify(11),
         classify(5), classify_run(2), classify_run(10), classify_run(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn classify(mut seed: i32) -> i32 {
// CHECK-NEXT:     let mut v: u128 = (340282366920938463463374607431768211440 as u128)
// CHECK-NEXT:         .wrapping_add((seed as i128) as u128);
// CHECK-NEXT:     match v {
// CHECK-NEXT:         340282366920938463463374607431768211441
// CHECK-NEXT:         | 340282366920938463463374607431768211443 => {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         340282366920938463463374607431768211450..=340282366920938463463374607431768211453 => {
// CHECK-NEXT:             return 2 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn classify_run(mut seed: i32) -> i32 {
// CHECK-NEXT:     let mut v: u128 = (340282366920938463463374607431768211440 as u128)
// CHECK-NEXT:         .wrapping_add((seed as i128) as u128);
// CHECK-NEXT:     match v {
// CHECK-NEXT:         340282366920938463463374607431768211441..=340282366920938463463374607431768211443 => {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         340282366920938463463374607431768211450 => {
// CHECK-NEXT:             return 2 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
