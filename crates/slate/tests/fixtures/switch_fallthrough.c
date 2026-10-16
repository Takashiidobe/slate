#include <stdio.h>

// @check-fn-begin
int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 10;
  case 2:
    out += 20;
    break;
  case 3:
  case 4:
    out += 40;
    break;
  default:
    out += 90;
  }
  return out;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d\n", score(1), score(2), score(3), score(4), score(8));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn score(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0 as i32;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             out += 10 as i32;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + (20 as i32);
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             out += 20 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         3 | 4 => {
// CHECK-NEXT:             out += 40 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             out += 90 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
