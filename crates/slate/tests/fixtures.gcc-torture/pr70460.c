/* { dg-require-effective-target indirect_jumps } */
/* { dg-require-effective-target label_values } */
/* { dg-skip-if "label differences not supported" { avr-*-* } } */

/* PR rtl-optimization/70460 */

int c;

__attribute__((noinline, noclone)) void foo(int x) {
  static int b[] = {&&lab1 - &&lab0, &&lab2 - &&lab0};
  void      *a   = &&lab0 + b[x];
  goto      *a;
lab1:
  c += 2;
lab2:
  c++;
lab0:;
}

int main() {
  foo(0);
  if (c != 3)
    __builtin_abort();
  foo(1);
  if (c != 4)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target indirect_jumps }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target label_values }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-skip-if "label differences not supported" { avr-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR rtl-optimization/70460
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut ()> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *mut () = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut *mut ();
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     {}
// CHECK-NEXT:                     break '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], unsafe { c });
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         c = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_3]], unsafe { c });
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_2]], *[[SLATE_SLOT_3]] + (2 as i32));
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         c = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_4]],
// CHECK-NEXT:                         (unsafe {
// CHECK-NEXT:                             (5usize as *const u8).wrapping_offset(
// CHECK-NEXT:                                 (unsafe {
// CHECK-NEXT:                                     *unsafe {
// CHECK-NEXT:                                         unsafe { std::ptr::addr_of_mut!(b) as *mut i32 }
// CHECK-NEXT:                                             .offset(x as isize)
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }) as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         }) as *mut (),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     match *[[SLATE_SLOT_4]] as usize {
// CHECK-NEXT:                         3 => {
// CHECK-NEXT:                             __slate_state = 2usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
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
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [i32; 2] = [(-2 as i64) as i32, (-1 as i64) as i32];
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(0 as i32);
// CHECK-NEXT:     if (unsafe { c }) != (3 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(1 as i32);
// CHECK-NEXT:     if (unsafe { c }) != (4 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
