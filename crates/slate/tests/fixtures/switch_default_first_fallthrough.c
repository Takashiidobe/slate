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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn score(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0 as i32;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (10 as i32);
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_3:_v[0-9]+]]: i32 = [[V_2]] + (20 as i32);
// CHECK-NEXT:             out = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         6 => {
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (20 as i32);
// CHECK-NEXT:             out = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         7 => {
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (40 as i32);
// CHECK-NEXT:             out = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:             out = [[V_9]];
// CHECK-NEXT:             let mut [[V_10:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_11:_v[0-9]+]]: i32 = [[V_10]] + (10 as i32);
// CHECK-NEXT:             out = [[V_11]];
// CHECK-NEXT:             let mut [[V_12:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_13:_v[0-9]+]]: i32 = [[V_12]] + (20 as i32);
// CHECK-NEXT:             out = [[V_13]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
