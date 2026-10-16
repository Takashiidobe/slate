#include <stdio.h>

// @check-fn-begin
int choose(int value) {
  if (value < 0)
    return -1;
  return value + 2;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", choose(-4), choose(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn choose(value: i32) -> i32 {
// CHECK-NEXT:     if value < (0 as i32) {
// CHECK-NEXT:         return -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     value + (2 as i32)
// CHECK: {{^}}}
// SLATE-FILECHECK-END
