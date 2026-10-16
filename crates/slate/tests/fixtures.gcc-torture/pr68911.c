extern void abort(void);

char  a;
int   b, c;
short d;

int main() {
  unsigned e       = 2;
  unsigned timeout = 0;

  for (; c < 2; c++) {
    int f = ~e / 7;
    if (f)
      a = e = ~(b && d);
    while (e < 94) {
      e++;
      if (++timeout > 100)
        goto die;
    }
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
// CHECK-NEXT: static mut a: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]])
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_9:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_9:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_9]]) as *mut u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_9]], (2 as i32) as u32);
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_8]], (0 as i32) as u32);
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if (unsafe { c }) < (2 as i32) {
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_7]],
// CHECK-NEXT:                         (!(*[[SLATE_SLOT_9]]) / ((7 as i32) as u32)) as i32,
// CHECK-NEXT:                     );
// CHECK-NEXT:                     if *[[SLATE_SLOT_7]] != (0 as i32) {
// CHECK-NEXT:                         std::ptr::write(
// CHECK-NEXT:                             [[SLATE_SLOT_6]],
// CHECK-NEXT:                             !(((unsafe { b }) != (0 as i32)
// CHECK-NEXT:                                 && (unsafe { d }) != (0 as i16)) as i32) as u32,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         *[[SLATE_SLOT_9]] = *[[SLATE_SLOT_6]];
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             a = (*[[SLATE_SLOT_6]] as u8) as i8;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     loop {
// CHECK-NEXT:                         if *[[SLATE_SLOT_9]] < ((94 as i32) as u32) {
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_5]], *[[SLATE_SLOT_9]]);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_4]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_5]]).wrapping_add((1 as i32) as u32),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             *[[SLATE_SLOT_9]] = *[[SLATE_SLOT_4]];
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_8]]);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_2]],
// CHECK-NEXT:                                 (*[[SLATE_SLOT_3]]).wrapping_add((1 as i32) as u32),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             *[[SLATE_SLOT_8]] = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                             if *[[SLATE_SLOT_2]] > ((100 as i32) as u32) {
// CHECK-NEXT:                                 break '[[JOIN_0]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], unsafe { c });
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         c = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { abort() };
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
