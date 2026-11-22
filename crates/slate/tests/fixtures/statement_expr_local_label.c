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
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     let mut first: i32 = 0;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut result: i32 = 0;
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = value + 1;
// CHECK-NEXT:         value = [[V_1]];
// CHECK-NEXT:         if [[V_1]] == 1 {
// CHECK-NEXT:             result = 17;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             result = -5;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = result;
// CHECK-NEXT:     }
// CHECK-NEXT:     first = [[V_0]];
// CHECK-NEXT:     let mut second: i32 = 0;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32;
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = value + 1;
// CHECK-NEXT:     value = [[V_3]];
// CHECK-NEXT:     if [[V_3]] == 9 {
// CHECK-NEXT:         result = 17;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         result = -5;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_2]] = result;
// CHECK-NEXT:     second = [[V_2]];
// CHECK-NEXT:     println!("{} {} {}", first, second, value);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
