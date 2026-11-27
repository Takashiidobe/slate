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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i64; 3]> = __SlateAlign16([4i64, 9i64, 16i64]);
// CHECK-NEXT:     let mut index: i32 = 0;
// CHECK-NEXT:     let old: i64 = EXCHANGE!(
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = index;
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: i32 = [[V_0]] + 1;
// CHECK-NEXT:             index = [[V_1]];
// CHECK-NEXT:             unsafe { values.0.as_mut_ptr().offset([[V_0]] as isize) }
// CHECK-NEXT:         },
// CHECK-NEXT:         25i64
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%ld %ld %d\n".as_ptr(), old, values.0[0], index) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
