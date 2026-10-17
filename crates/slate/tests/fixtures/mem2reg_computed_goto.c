#include <stdio.h>

// @check-fn-begin
void walk(int *position) {
  static void *labels[] = {&&again, &&done};
  goto        *labels[*position];
again:
  position++;
  goto *labels[*position];
done:
  return;
}
// @check-fn-end

int main(void) {
  int path[] = {0, 1};
  walk(path);
  printf("done\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn walk(mut position: *mut i32) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut *mut i32;
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], position);
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_0]],
// CHECK-NEXT:                         unsafe { (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize) },
// CHECK-NEXT:                     );
// CHECK-NEXT:                     position = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(labels.0) as *mut *mut () }
// CHECK-NEXT:                                 .offset((unsafe { *position }) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(labels.0) as *mut *mut () }
// CHECK-NEXT:                                 .offset((unsafe { *position }) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
