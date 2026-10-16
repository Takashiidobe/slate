#include <stdio.h>

// @check-fn-begin
int main() {
  volatile int pick_a = 1;
  volatile int pick_b = 0;
  int          x      = 0;
  int          y      = 0;

  if (pick_a)
    goto a2;
a1:
  x = x + 1;
  if (x < 3)
    goto a2;
  goto second;
a2:
  x = x + 2;
  if (x < 4)
    goto a1;

second:
  if (pick_b)
    goto b2;
b1:
  y = y + 3;
  if (y < 9)
    goto b2;
  goto done;
b2:
  y = y + 5;
  if (y < 11)
    goto b1;

done:
  printf("%d %d\n", x, y);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i32;
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_3]], 1 as i32);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_2]], 0 as i32);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_1]], 0 as i32);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_0]], 0 as i32);
// CHECK-NEXT:             if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(* [[SLATE_SLOT_3]])) })
// CHECK-NEXT:                 != (0 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 __slate_state = 4;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 __slate_state = 5;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_1]] += 2 as i32;
// CHECK-NEXT:                     if *[[SLATE_SLOT_1]] < (4 as i32) {
// CHECK-NEXT:                         __slate_state = 5;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     *[[SLATE_SLOT_1]] += 1 as i32;
// CHECK-NEXT:                     if *[[SLATE_SLOT_1]] < (3 as i32) {
// CHECK-NEXT:                         __slate_state = 4;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(* [[SLATE_SLOT_2]])) })
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             __slate_state = 1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             __slate_state = 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_0]] += 5 as i32;
// CHECK-NEXT:                     if *[[SLATE_SLOT_0]] < (11 as i32) {
// CHECK-NEXT:                         __slate_state = 2;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     *[[SLATE_SLOT_0]] += 3 as i32;
// CHECK-NEXT:                     if *[[SLATE_SLOT_0]] < (9 as i32) {
// CHECK-NEXT:                         __slate_state = 1;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(c"%d %d\n".as_ptr() as *const i8, *[[SLATE_SLOT_1]], *[[SLATE_SLOT_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
