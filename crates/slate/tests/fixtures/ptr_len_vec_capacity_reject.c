#include <stdio.h>
#include <stdlib.h>

// @check-fn-begin
static int consume_extra_capacity(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i]  = i + 1;
    sum       += values[i];
  }
  free(values);
  return sum;
}
// @check-fn-end

int main(void) {
  int  len    = 3;
  int *values = malloc((len + 1) * sizeof(int));
  int  sum    = consume_extra_capacity(values, len);
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn consume_extra_capacity(values: *mut i32, len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { values.offset(i as isize) } = i + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         sum += unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(values as *mut ()) };
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// SLATE-FILECHECK-END
