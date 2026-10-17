#include <stdio.h>

static int parameter_bound(int length, int values[length++]) {
  return length + values[0];
}

// @check-fn-begin
int main(void) {
  int           bound       = 3;
  unsigned long evaluated   = sizeof(int[bound++]);
  unsigned long unevaluated = sizeof(int (*)[bound++]);
  int           values[]    = {7, 8, 9};
  int           parameter   = parameter_bound(3, values);
  printf("%lu %lu %d %d\n", evaluated, unevaluated, bound, parameter);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut bound: i32 = 3;
// CHECK-NEXT:     let mut evaluated: u64 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = bound;
// CHECK-NEXT:     bound = [[V_0]] + 1;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u64 = ([[V_0]] as i64) as u64;
// CHECK-NEXT:     evaluated = [[V_1]].wrapping_mul(4);
// CHECK-NEXT:     let unevaluated: u64 = 8;
// CHECK-NEXT:     let mut values: [i32; 3] = [7, 8, 9];
// CHECK-NEXT:     let parameter: i32 = parameter_bound(3 as i32, values.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%lu %lu %d %d\n".as_ptr(), evaluated, unevaluated, bound, parameter)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
