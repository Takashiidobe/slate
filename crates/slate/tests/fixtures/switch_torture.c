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
// CHECK-NEXT:             out += 1;
// CHECK-NEXT:             out += 2;
// CHECK-NEXT:             if out <= 50 {
// CHECK-NEXT:                 out += 3;
// CHECK-NEXT:                 out += 4;
// CHECK-NEXT:                 if x % 2 != 0 {
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out += 6;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         0 | 1 => {
// CHECK-NEXT:             out += 1;
// CHECK-NEXT:             out += 2;
// CHECK-NEXT:             if out <= 50 {
// CHECK-NEXT:                 out += 3;
// CHECK-NEXT:                 out += 4;
// CHECK-NEXT:                 if x % 2 != 0 {
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out += 6;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         2..=100 => {
// CHECK-NEXT:             out += 2;
// CHECK-NEXT:             if out <= 50 {
// CHECK-NEXT:                 out += 3;
// CHECK-NEXT:                 out += 4;
// CHECK-NEXT:                 if x % 2 != 0 {
// CHECK-NEXT:                     out += 5;
// CHECK-NEXT:                     out += 6;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         101 => {
// CHECK-NEXT:             out += 4;
// CHECK-NEXT:             if x % 2 != 0 {
// CHECK-NEXT:                 out += 5;
// CHECK-NEXT:                 out += 6;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         200..=500 | 600 | 700..=900 => {
// CHECK-NEXT:             out += 6;
// CHECK-NEXT:         }
// CHECK-NEXT:         999 => {
// CHECK-NEXT:             out += 7;
// CHECK-NEXT:             out += 8;
// CHECK-NEXT:         }
// CHECK-NEXT:         1000 => {
// CHECK-NEXT:             out += 8;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             out += 5;
// CHECK-NEXT:             out += 6;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK: {{^}}}
// SLATE-FILECHECK-END
