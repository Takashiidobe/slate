#include <stdio.h>

#define EXCHANGE(pointer, replacement)                                         \
  ({                                                                           \
    __auto_type exchange_pointer = (pointer);                                  \
    __auto_type exchange_value   = *exchange_pointer;                          \
    *exchange_pointer            = (replacement);                              \
    exchange_value;                                                            \
  })

// @check-fn-begin
int main(void) {
  long values[] = {4, 9, 16};
  int  index    = 0;
  long old      = EXCHANGE(&values[index++], 25L);
  printf("%ld %ld %d\n", old, values[0], index);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i64; 3]> = __SlateAlign16([
// CHECK-NEXT:         (4 as i32) as i64,
// CHECK-NEXT:         (9 as i32) as i64,
// CHECK-NEXT:         (16 as i32) as i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut index: i32 = 0 as i32;
// CHECK-NEXT:     let mut old: i64 = 0 as i64;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i64;
// CHECK-NEXT:     let mut exchange_pointer: *mut i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = index;
// CHECK-NEXT:     index = [[V_1]] + (1 as i32);
// CHECK-NEXT:     exchange_pointer = unsafe {
// CHECK-NEXT:         (values.0.as_mut_ptr() as *mut i64).offset([[V_1]] as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let exchange_value: i64 = unsafe { *exchange_pointer };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *exchange_pointer = 25 as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] = exchange_value;
// CHECK-NEXT:     old = [[V_0]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld %ld %d\n".as_ptr() as *const i8, old, values.0[0], index) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
