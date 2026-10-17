#include <stdio.h>

// @check-fn-begin
int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 1;
  default:
    out += 2;
  case 3:
    out += 3;
    break;
  case 4:
    out += 4;
  }
  return out;
}
// @check-fn-end

// @check-fn-begin
int shared(int x) {
  int out = 0;
  switch (x) {
  case 2:
  default:
    out += 10;
  case 5:
    out += 20;
  }
  return out;
}
// @check-fn-end

int main(void) {
  for (int i = 0; i < 7; i++)
    printf("%d,%d ", score(i), shared(i));
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn score(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             out += 1;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + 2;
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:             let [[V_2:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_3:_v[0-9]+]]: i32 = [[V_2]] + 3;
// CHECK-NEXT:             out = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             out += 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             out += 4;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             out += 2;
// CHECK-NEXT:             let [[V_4:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_5:_v[0-9]+]]: i32 = [[V_4]] + 3;
// CHECK-NEXT:             out = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shared(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             out += 20;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             out += 10;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + 20;
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
