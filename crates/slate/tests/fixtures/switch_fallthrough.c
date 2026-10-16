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
// CHECK-LABEL: {{^}}fn score(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0 as i32;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (10 as i32);
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_3:_v[0-9]+]]: i32 = [[V_2]] + (20 as i32);
// CHECK-NEXT:             out = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (20 as i32);
// CHECK-NEXT:             out = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         3 | 4 => {
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (40 as i32);
// CHECK-NEXT:             out = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (90 as i32);
// CHECK-NEXT:             out = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
