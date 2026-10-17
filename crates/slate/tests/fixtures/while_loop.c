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
// CHECK-LABEL: {{^}}fn sum_while(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     while i <= n {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
