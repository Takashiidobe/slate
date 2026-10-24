#include <stdio.h>

#define CHECK_VALUE(expression)                                                \
  ({                                                                           \
    __label__ failed, done;                                                    \
    int result;                                                                \
    if (!(expression))                                                         \
      goto failed;                                                             \
    result = 17;                                                               \
    goto done;                                                                 \
  failed:                                                                      \
    result = -5;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

// @check-fn-begin
int main(void) {
  int value  = 0;
  int first  = CHECK_VALUE(++value == 1);
  int second = CHECK_VALUE(++value == 9);
  printf("%d %d %d\n", first, second, value);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[RESULT_0:result_[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut first: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         value = 0;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = value + 1;
// CHECK-NEXT:         value = [[V_0]];
// CHECK-NEXT:         if [[V_0]] != 1 {
// CHECK-NEXT:             [[RESULT_0]] = -5;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[RESULT_0]] = 17;
// CHECK-NEXT:         }
// CHECK-NEXT:         first = [[RESULT_0]];
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = value + 1;
// CHECK-NEXT:         value = [[V_1]];
// CHECK-NEXT:         if [[V_1]] != 9 {
// CHECK-NEXT:             result = -5;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             result = 17;
// CHECK-NEXT:         }
// CHECK-NEXT:         let second: i32 = result;
// CHECK-NEXT:         println!("{} {} {}", first, second, value);
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
