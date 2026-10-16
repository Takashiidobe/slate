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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn consume_extra_capacity(mut values: *mut i32, mut len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { values.offset(i as isize) } = i + (1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:         sum = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(values as *mut ()) };
// CHECK-NEXT:     sum
// CHECK: {{^}}}
// SLATE-FILECHECK-END
