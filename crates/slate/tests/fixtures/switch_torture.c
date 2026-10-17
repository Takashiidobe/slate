#include <stdio.h>

// @check-fn-begin
int f(int x) {
  int out = 0;
  switch (x) {
  case -500 ... - 1:
    out += 100;
  case 0:
  case 1:
    out += 1;
  case 2 ... 100:
    out += 2;
    if (out > 50) {
      break;
    }
    out += 3;
  case 101:
    out += 4;
    if (x % 2 == 0) {
      break;
    }
  default:
    out += 5;
  case 200 ... 500:
  case 600:
  case 700 ... 900:
    out += 6;
    break;
  case 999:
    out += 7;
  case 1000:
    out += 8;
  }
  return out;
}
// @check-fn-end

int main(void) {
  for (int x = -600; x < 1001; x++)
    printf("%d,", f(x));
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn f(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         -500..=-1 => {
// CHECK-NEXT:             out += 100;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + 1;
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:             let [[V_2:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_3:_v[0-9]+]]: i32 = [[V_2]] + 2;
// CHECK-NEXT:             out = [[V_3]];
// CHECK-NEXT:             if out > 50 {} else {
// CHECK-NEXT:                 let [[V_4:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_5:_v[0-9]+]]: i32 = [[V_4]] + 3;
// CHECK-NEXT:                 out = [[V_5]];
// CHECK-NEXT:                 let [[V_6:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_7:_v[0-9]+]]: i32 = [[V_6]] + 4;
// CHECK-NEXT:                 out = [[V_7]];
// CHECK-NEXT:                 if x % 2 == 0 {} else {
// CHECK-NEXT:                     let [[V_8:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let [[V_9:_v[0-9]+]]: i32 = [[V_8]] + 5;
// CHECK-NEXT:                     out = [[V_9]];
// CHECK-NEXT:                     let [[V_10:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let [[V_11:_v[0-9]+]]: i32 = [[V_10]] + 6;
// CHECK-NEXT:                     out = [[V_11]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         0 | 1 => {
// CHECK-NEXT:             out += 1;
// CHECK-NEXT:             let [[V_12:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_13:_v[0-9]+]]: i32 = [[V_12]] + 2;
// CHECK-NEXT:             out = [[V_13]];
// CHECK-NEXT:             if out > 50 {} else {
// CHECK-NEXT:                 let [[V_14:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_15:_v[0-9]+]]: i32 = [[V_14]] + 3;
// CHECK-NEXT:                 out = [[V_15]];
// CHECK-NEXT:                 let [[V_16:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_17:_v[0-9]+]]: i32 = [[V_16]] + 4;
// CHECK-NEXT:                 out = [[V_17]];
// CHECK-NEXT:                 if x % 2 == 0 {} else {
// CHECK-NEXT:                     let [[V_18:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let [[V_19:_v[0-9]+]]: i32 = [[V_18]] + 5;
// CHECK-NEXT:                     out = [[V_19]];
// CHECK-NEXT:                     let [[V_20:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let [[V_21:_v[0-9]+]]: i32 = [[V_20]] + 6;
// CHECK-NEXT:                     out = [[V_21]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         2..=100 => {
// CHECK-NEXT:             out += 2;
// CHECK-NEXT:             if out > 50 {} else {
// CHECK-NEXT:                 out += 3;
// CHECK-NEXT:                 let [[V_22:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_23:_v[0-9]+]]: i32 = [[V_22]] + 4;
// CHECK-NEXT:                 out = [[V_23]];
// CHECK-NEXT:                 if x % 2 == 0 {} else {
// CHECK-NEXT:                     let [[V_24:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let [[V_25:_v[0-9]+]]: i32 = [[V_24]] + 5;
// CHECK-NEXT:                     out = [[V_25]];
// CHECK-NEXT:                     let [[V_26:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let [[V_27:_v[0-9]+]]: i32 = [[V_26]] + 6;
// CHECK-NEXT:                     out = [[V_27]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         101 => {
// CHECK-NEXT:             out += 4;
// CHECK-NEXT:             if x % 2 == 0 {} else {
// CHECK-NEXT:                 let [[V_28:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_29:_v[0-9]+]]: i32 = [[V_28]] + 5;
// CHECK-NEXT:                 out = [[V_29]];
// CHECK-NEXT:                 let [[V_30:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let [[V_31:_v[0-9]+]]: i32 = [[V_30]] + 6;
// CHECK-NEXT:                 out = [[V_31]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         200..=500 | 600 | 700..=900 => {
// CHECK-NEXT:             out += 6;
// CHECK-NEXT:         }
// CHECK-NEXT:         999 => {
// CHECK-NEXT:             out += 7;
// CHECK-NEXT:             let [[V_32:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_33:_v[0-9]+]]: i32 = [[V_32]] + 8;
// CHECK-NEXT:             out = [[V_33]];
// CHECK-NEXT:         }
// CHECK-NEXT:         1000 => {
// CHECK-NEXT:             out += 8;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             out += 5;
// CHECK-NEXT:             let [[V_34:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_35:_v[0-9]+]]: i32 = [[V_34]] + 6;
// CHECK-NEXT:             out = [[V_35]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
