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
// CHECK-NEXT:     let mut bound: i32 = 3 as i32;
// CHECK-NEXT:     let mut evaluated: u64 = 0 as u64;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = bound;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     bound = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = ([[V_0]] as i64) as u64;
// CHECK-NEXT:     evaluated = [[V_2]].wrapping_mul(4 as u64);
// CHECK-NEXT:     let mut unevaluated: u64 = 8 as u64;
// CHECK-NEXT:     let mut values: [i32; 3] = [7 as i32, 8 as i32, 9 as i32];
// CHECK-NEXT:     let mut parameter: i32 = parameter_bound(3 as i32, values.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lu %lu %d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             evaluated,
// CHECK-NEXT:             unevaluated,
// CHECK-NEXT:             bound,
// CHECK-NEXT:             parameter,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
