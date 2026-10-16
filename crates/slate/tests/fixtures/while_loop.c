#include <stdio.h>

// @check-fn-begin
static int sum_while(int n) {
  int total = 0;
  int i     = 1;
  while (i <= n) {
    total += i;
    i++;
  }
  return total;
}
// @check-fn-end

int main(void) {
  printf("%d\n", sum_while(5));
  printf("%d\n", sum_while(1));
  printf("%d\n", sum_while(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn sum_while(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= n {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + i;
// CHECK-NEXT:         total = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
