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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn f(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0 as i32;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         -500..=-1 => {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (100 as i32);
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_3:_v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             out = [[V_3]];
// CHECK-NEXT:             let mut [[V_4:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_5:_v[0-9]+]]: i32 = [[V_4]] + (2 as i32);
// CHECK-NEXT:             out = [[V_5]];
// CHECK-NEXT:             if out > (50 as i32) {} else {
// CHECK-NEXT:                 let mut [[V_6:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_7:_v[0-9]+]]: i32 = [[V_6]] + (3 as i32);
// CHECK-NEXT:                 out = [[V_7]];
// CHECK-NEXT:                 let mut [[V_8:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_9:_v[0-9]+]]: i32 = [[V_8]] + (4 as i32);
// CHECK-NEXT:                 out = [[V_9]];
// CHECK-NEXT:                 if x % (2 as i32) == (0 as i32) {} else {
// CHECK-NEXT:                     let mut [[V_10:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let mut [[V_11:_v[0-9]+]]: i32 = [[V_10]] + (5 as i32);
// CHECK-NEXT:                     out = [[V_11]];
// CHECK-NEXT:                     let mut [[V_12:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let mut [[V_13:_v[0-9]+]]: i32 = [[V_12]] + (6 as i32);
// CHECK-NEXT:                     out = [[V_13]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         0 | 1 => {
// CHECK-NEXT:             let mut [[V_14:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:             out = [[V_15]];
// CHECK-NEXT:             let mut [[V_16:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_17:_v[0-9]+]]: i32 = [[V_16]] + (2 as i32);
// CHECK-NEXT:             out = [[V_17]];
// CHECK-NEXT:             if out > (50 as i32) {} else {
// CHECK-NEXT:                 let mut [[V_18:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_19:_v[0-9]+]]: i32 = [[V_18]] + (3 as i32);
// CHECK-NEXT:                 out = [[V_19]];
// CHECK-NEXT:                 let mut [[V_20:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_21:_v[0-9]+]]: i32 = [[V_20]] + (4 as i32);
// CHECK-NEXT:                 out = [[V_21]];
// CHECK-NEXT:                 if x % (2 as i32) == (0 as i32) {} else {
// CHECK-NEXT:                     let mut [[V_22:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let mut [[V_23:_v[0-9]+]]: i32 = [[V_22]] + (5 as i32);
// CHECK-NEXT:                     out = [[V_23]];
// CHECK-NEXT:                     let mut [[V_24:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let mut [[V_25:_v[0-9]+]]: i32 = [[V_24]] + (6 as i32);
// CHECK-NEXT:                     out = [[V_25]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         2..=100 => {
// CHECK-NEXT:             let mut [[V_26:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]] + (2 as i32);
// CHECK-NEXT:             out = [[V_27]];
// CHECK-NEXT:             if out > (50 as i32) {} else {
// CHECK-NEXT:                 let mut [[V_28:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_29:__v[0-9]+]]: i32 = [[V_28]] + (3 as i32);
// CHECK-NEXT:                 out = [[V_29]];
// CHECK-NEXT:                 let mut [[V_30:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_31:_v[0-9]+]]: i32 = [[V_30]] + (4 as i32);
// CHECK-NEXT:                 out = [[V_31]];
// CHECK-NEXT:                 if x % (2 as i32) == (0 as i32) {} else {
// CHECK-NEXT:                     let mut [[V_32:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let mut [[V_33:_v[0-9]+]]: i32 = [[V_32]] + (5 as i32);
// CHECK-NEXT:                     out = [[V_33]];
// CHECK-NEXT:                     let mut [[V_34:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                     let mut [[V_35:_v[0-9]+]]: i32 = [[V_34]] + (6 as i32);
// CHECK-NEXT:                     out = [[V_35]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         101 => {
// CHECK-NEXT:             let mut [[V_36:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_37:__v[0-9]+]]: i32 = [[V_36]] + (4 as i32);
// CHECK-NEXT:             out = [[V_37]];
// CHECK-NEXT:             if x % (2 as i32) == (0 as i32) {} else {
// CHECK-NEXT:                 let mut [[V_38:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_39:_v[0-9]+]]: i32 = [[V_38]] + (5 as i32);
// CHECK-NEXT:                 out = [[V_39]];
// CHECK-NEXT:                 let mut [[V_40:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:                 let mut [[V_41:_v[0-9]+]]: i32 = [[V_40]] + (6 as i32);
// CHECK-NEXT:                 out = [[V_41]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         200..=500 | 600 | 700..=900 => {
// CHECK-NEXT:             let mut [[V_42:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_43:__v[0-9]+]]: i32 = [[V_42]] + (6 as i32);
// CHECK-NEXT:             out = [[V_43]];
// CHECK-NEXT:         }
// CHECK-NEXT:         999 => {
// CHECK-NEXT:             let mut [[V_44:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_45:__v[0-9]+]]: i32 = [[V_44]] + (7 as i32);
// CHECK-NEXT:             out = [[V_45]];
// CHECK-NEXT:             let mut [[V_46:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_47:_v[0-9]+]]: i32 = [[V_46]] + (8 as i32);
// CHECK-NEXT:             out = [[V_47]];
// CHECK-NEXT:         }
// CHECK-NEXT:         1000 => {
// CHECK-NEXT:             let mut [[V_48:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_49:__v[0-9]+]]: i32 = [[V_48]] + (8 as i32);
// CHECK-NEXT:             out = [[V_49]];
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             let mut [[V_50:__v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_51:__v[0-9]+]]: i32 = [[V_50]] + (5 as i32);
// CHECK-NEXT:             out = [[V_51]];
// CHECK-NEXT:             let mut [[V_52:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let mut [[V_53:_v[0-9]+]]: i32 = [[V_52]] + (6 as i32);
// CHECK-NEXT:             out = [[V_53]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
