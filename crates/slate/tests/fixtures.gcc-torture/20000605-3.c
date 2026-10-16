void abort(void);

struct F {
  int x;
  int y;
};

int main() {
  int timeout = 0;
  int x       = 0;
  while (1) {
    const struct F i = {
        x++,
    };
    if (i.x > 0)
      break;
    if (++timeout > 5)
      goto die;
  }
  return 0;
die:
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct F {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<F> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut F = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut F;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: {
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_6]], 0 as i32);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_5]], 0 as i32);
// CHECK-NEXT:             '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:                 loop {
// CHECK-NEXT:                     if (1 as i32) != (0 as i32) {
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_5]]);
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_2]], *[[SLATE_SLOT_3]] + (1 as i32));
// CHECK-NEXT:                         *[[SLATE_SLOT_5]] = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                         *[[SLATE_SLOT_4]] = {
// CHECK-NEXT:                             let mut [[T_0:__t[0-9]+]]: F = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                             [[T_0]].x = *[[SLATE_SLOT_3]];
// CHECK-NEXT:                             [[T_0]]
// CHECK-NEXT:                         };
// CHECK-NEXT:                         if (*[[SLATE_SLOT_4]]).x > (0 as i32) {
// CHECK-NEXT:                             break '[[JOIN_0]];
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_6]]);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_0]],
// CHECK-NEXT:                                 *[[SLATE_SLOT_1]] + (1 as i32),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             *[[SLATE_SLOT_6]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                             if *[[SLATE_SLOT_0]] > (5 as i32) {
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break '[[JOIN_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:                 break '__slate_dispatch;
// CHECK-NEXT:             }
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
