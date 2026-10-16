/* { dg-require-effective-target int32plus } */
/* PR tree-optimization/112581 */
/* reassociation, used to combine 2 bb to together,
   that made an unitialized variable unconditional used
   which then at runtime would cause an infinite loop.  */
int a = -1, b = 2501896061, c, d, e, f = 3, g;
int main() {
  unsigned h;
  int      i;
  d = 0;
  for (; d < 1; d++) {
    int j = ~-((6UL ^ a) / b);
    if (b)
    L:
      if (!f)
        continue;
    if (c)
      i = 1;
    if (j) {
      i = 0;
      while (e)
        ;
    }
    g = -1 % b;
    h = ~(b || h);
    f = g || 0;
    a = a || 0;
    if (!a)
      h = 0;
    while (h > 4294967294)
      if (i)
        break;
    if (c)
      goto L;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target int32plus }
// CHECK-NEXT: ///
// CHECK-NEXT: /// PR tree-optimization/112581
// CHECK-NEXT: ///
// CHECK-NEXT: /// reassociation, used to combine 2 bb to together,
// CHECK-NEXT: /// that made an unitialized variable unconditional used
// CHECK-NEXT: /// which then at runtime would cause an infinite loop.
// CHECK-NEXT: static mut a: i32 = -(1 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = (2501896061 as i64) as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = 3 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
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
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut u32;
// CHECK-NEXT:     let mut __slate_state: usize = 16usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 0 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     if (unsafe { d }) < (1 as i32) {
// CHECK-NEXT:                         __slate_state = 15usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 0usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], unsafe { d });
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         d = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     if (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 14usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     if ((*[[SLATE_SLOT_4]] as u64) as i64) > (4294967294 as i64) {
// CHECK-NEXT:                         __slate_state = 5usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     if *[[SLATE_SLOT_3]] != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 4usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_4]] = (0 as i32) as u32;
// CHECK-NEXT:                     __slate_state = 4usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 7 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         g = -(1 as i32) % unsafe { b };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     *[[SLATE_SLOT_4]] = !(((unsafe { b }) != (0 as i32)
// CHECK-NEXT:                         || *[[SLATE_SLOT_4]] != (0 as u32))
// CHECK-NEXT:                         as i32) as u32;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         f = ((unsafe { g }) != (0 as i32) || (0 as i32) != (0 as i32)) as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         a = ((unsafe { a }) != (0 as i32) || (0 as i32) != (0 as i32)) as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if !((unsafe { a }) != (0 as i32)) {
// CHECK-NEXT:                         __slate_state = 6usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 4usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 8 => {
// CHECK-NEXT:                     if (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 9usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 7usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 9 => {
// CHECK-NEXT:                     {}
// CHECK-NEXT:                     __slate_state = 8usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 10 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_3]] = 0 as i32;
// CHECK-NEXT:                     __slate_state = 8usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 11 => {
// CHECK-NEXT:                     if *[[SLATE_SLOT_2]] != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 10usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 7usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 12 => {
// CHECK-NEXT:                     *[[SLATE_SLOT_3]] = 1 as i32;
// CHECK-NEXT:                     __slate_state = 11usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 13 => {
// CHECK-NEXT:                     if (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 12usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 11usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 14 => {
// CHECK-NEXT:                     if !((unsafe { f }) != (0 as i32)) {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 13usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 15 => {
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_2]],
// CHECK-NEXT:                         (!(((6 as u64) ^ (((unsafe { a }) as i64) as u64))
// CHECK-NEXT:                             / (((unsafe { b }) as i64) as u64))
// CHECK-NEXT:                             .wrapping_neg() as u32) as i32,
// CHECK-NEXT:                     );
// CHECK-NEXT:                     if (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 14usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 13usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 16 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         d = 0 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
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
