#include <stdio.h>

// @check-fn-begin
int score(int x) {
  int out = 0;
  switch (x) {
  default:
    out += 1;
  case 5:
    out += 10;
  case 6:
    out += 20;
    break;
  case 7:
    out += 40;
  }
  return out;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d\n", score(5), score(6), score(7), score(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn score(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             out += 10;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + 20;
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         6 => {
// CHECK-NEXT:             out += 20;
// CHECK-NEXT:         }
// CHECK-NEXT:         7 => {
// CHECK-NEXT:             out += 40;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             out += 1;
// CHECK-NEXT:             let [[V_2:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_3:_v[0-9]+]]: i32 = [[V_2]] + 10;
// CHECK-NEXT:             out = [[V_3]];
// CHECK-NEXT:             let [[V_4:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_5:_v[0-9]+]]: i32 = [[V_4]] + 20;
// CHECK-NEXT:             out = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
