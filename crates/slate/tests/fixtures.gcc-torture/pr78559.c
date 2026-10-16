/* PR rtl-optimization/78559 */

int g = 20;
int d = 0;

short fn2(int p1, int p2) { return p2 >= 2 || 5 >> p2 ? p1 : p1 << p2; }

int main() {
  int result = 0;
lbl_2582:
  if (g) {
    for (int c = -3; c; c++)
      result = fn2(1, g);
  } else {
    for (int i = 0; i < 2; i += 2)
      if (d)
        goto lbl_2582;
  }
  if (result != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/78559
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = 20 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2(mut p1: i32, mut p2: i32) -> i16 {
// CHECK-NEXT:     (if p2 >= (2 as i32) || (5 as i32) >> p2 != (0 as i32) {
// CHECK-NEXT:         p1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         p1 << p2
// CHECK-NEXT:     }) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_6]], 0 as i32);
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             '__loop_10: loop {
// CHECK-NEXT:                 if (unsafe { g }) != (0 as i32) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_5]], 0 as i32);
// CHECK-NEXT:                     loop {
// CHECK-NEXT:                         if *[[SLATE_SLOT_5]] < (2 as i32) {
// CHECK-NEXT:                             if (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:                                 continue '__loop_10;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 std::ptr::write([[SLATE_SLOT_4]], *[[SLATE_SLOT_5]]);
// CHECK-NEXT:                                 std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_4]] + (2 as i32));
// CHECK-NEXT:                                 *[[SLATE_SLOT_5]] = *[[SLATE_SLOT_3]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             break '[[JOIN_0]];
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_2]], -(3 as i32));
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if *[[SLATE_SLOT_2]] != (0 as i32) {
// CHECK-NEXT:                     *[[SLATE_SLOT_6]] = fn2(1 as i32, unsafe { g }) as i32;
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_2]]);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                     *[[SLATE_SLOT_2]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if *[[SLATE_SLOT_6]] != (1 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
