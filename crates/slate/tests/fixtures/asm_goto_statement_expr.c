#include <stdio.h>

#define CLASSIFY_ZERO(value)                                                   \
  ({                                                                           \
    __label__ zero, done;                                                      \
    int result = 0;                                                            \
    __asm__ goto("testl %0, %0\n\tjz %l[zero]" : : "r"(value) : "cc" : zero);  \
    result = 11;                                                               \
    goto done;                                                                 \
  zero:                                                                        \
    result = 13;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

// @check-fn-begin
int main(void) {
  printf("%d %d\n", CLASSIFY_ZERO(0), CLASSIFY_ZERO(9));
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     {}
// CHECK-NEXT:                     *[[SLATE_SLOT_1]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         printf(
// CHECK-NEXT:                             c"%d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:                             *[[SLATE_SLOT_3]],
// CHECK-NEXT:                             *[[SLATE_SLOT_1]],
// CHECK-NEXT:                         )
// CHECK-NEXT:                     };
// CHECK-NEXT:                     return 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_0]] = 13 as i32;
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_0]] = 11 as i32;
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     {}
// CHECK-NEXT:                     *[[SLATE_SLOT_3]] = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], 0 as i32);
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "testl {0:e}, {0:e}\n\tjz {1}", in (reg) (9 as i32) as u32,
// CHECK-NEXT:                             label { __slate_state = 1usize; }, options(nostack,
// CHECK-NEXT:                             att_syntax)
// CHECK-NEXT:                         );
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_2]] = 13 as i32;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_2]] = 11 as i32;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_2]], 0 as i32);
// CHECK-NEXT:                     __slate_state = 5usize;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "testl {0:e}, {0:e}\n\tjz {1}", in (reg) (0 as i32) as u32,
// CHECK-NEXT:                             label { __slate_state = 4usize; }, options(nostack,
// CHECK-NEXT:                             att_syntax)
// CHECK-NEXT:                         );
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
