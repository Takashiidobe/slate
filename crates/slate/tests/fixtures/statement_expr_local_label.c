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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(result) as *mut i32;
// CHECK-NEXT:     let mut result: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(result) as *mut i32;
// CHECK-NEXT:     let mut first: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         value = 0 as i32;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = value + (1 as i32);
// CHECK-NEXT:         value = [[V_0]];
// CHECK-NEXT:         if !([[V_0]] == (1 as i32)) {
// CHECK-NEXT:             *[[SLATE_SLOT_1]] = -(5 as i32);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             *[[SLATE_SLOT_1]] = 17 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         {}
// CHECK-NEXT:         first = *[[SLATE_SLOT_1]];
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = value + (1 as i32);
// CHECK-NEXT:         value = [[V_1]];
// CHECK-NEXT:         if !([[V_1]] == (9 as i32)) {
// CHECK-NEXT:             *[[SLATE_SLOT_0]] = -(5 as i32);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             *[[SLATE_SLOT_0]] = 17 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         {}
// CHECK-NEXT:         let second: i32 = *[[SLATE_SLOT_0]];
// CHECK-NEXT:         println!("{} {} {}", first, second, value);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
