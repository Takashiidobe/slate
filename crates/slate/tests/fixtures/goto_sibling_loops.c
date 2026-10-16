#include <stdio.h>

// @check-fn-begin
int main() {
  int i    = 0;
  int k    = 0;
  int sum  = 0;
  int prod = 1;
first:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto first;
  k = 1;
second:
  prod = prod * k;
  k    = k + 1;
  if (k < 5)
    goto second;
  printf("%d %d\n", sum, prod);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_3]], 0 as i32);
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_2]], 0 as i32);
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_1]], 0 as i32);
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_0]], 1 as i32);
// CHECK-NEXT:         loop {
// CHECK-NEXT:             *[[SLATE_SLOT_1]] += *[[SLATE_SLOT_3]];
// CHECK-NEXT:             *[[SLATE_SLOT_3]] += 1 as i32;
// CHECK-NEXT:             if !(*[[SLATE_SLOT_3]] < (5 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         *[[SLATE_SLOT_2]] = 1 as i32;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             *[[SLATE_SLOT_0]] *= *[[SLATE_SLOT_2]];
// CHECK-NEXT:             *[[SLATE_SLOT_2]] += 1 as i32;
// CHECK-NEXT:             if !(*[[SLATE_SLOT_2]] < (5 as i32)) {
// CHECK-NEXT:                 break;
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
