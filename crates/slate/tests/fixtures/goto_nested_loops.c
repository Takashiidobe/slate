#include <stdio.h>

// @check-fn-begin
int main() {
  int i     = 0;
  int j     = 0;
  int total = 0;
outer:
  j = 0;
inner:
  total = total + (i * j);
  j     = j + 1;
  if (j < 3)
    goto inner;
  i = i + 1;
  if (i < 4)
    goto outer;
  printf("%d\n", total);
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
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_2]], 0 as i32);
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_1]], 0 as i32);
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_0]], 0 as i32);
// CHECK-NEXT:         loop {
// CHECK-NEXT:             *[[SLATE_SLOT_1]] = 0 as i32;
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 *[[SLATE_SLOT_0]] += *[[SLATE_SLOT_2]] * *[[SLATE_SLOT_1]];
// CHECK-NEXT:                 *[[SLATE_SLOT_1]] += 1 as i32;
// CHECK-NEXT:                 if !(*[[SLATE_SLOT_1]] < (3 as i32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             *[[SLATE_SLOT_2]] += 1 as i32;
// CHECK-NEXT:             if !(*[[SLATE_SLOT_2]] < (4 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", * [[SLATE_SLOT_0]]);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
