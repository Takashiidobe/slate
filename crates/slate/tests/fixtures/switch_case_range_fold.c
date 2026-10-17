#include <stdio.h>

// @check-fn-begin
int classify(int c) {
  switch (c) {
  case 48:
  case 49:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 56:
  case 57:
    return 1;
  case 'a':
  case 'b':
  case 'c':
    return 2;
  case 100:
  case 200:
    return 3;
  case -3:
  case -2:
  case -1:
  case 7:
    return 4;
  default:
    return 0;
  }
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify('5'), classify('b'), classify(100),
         classify(200), classify(-2), classify(7), classify(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn classify(c: i32) -> i32 {
// CHECK-NEXT:     match c {
// CHECK-NEXT:         48..=57 => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         97..=99 => {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         100 | 200 => {
// CHECK-NEXT:             return 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         -3..=-1 | 7 => {
// CHECK-NEXT:             return 4;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
