/* PR rtl-optimization/119291 */

int  a;
long c;

__attribute__((noipa)) void foo(int x) {
  if (x != 0)
    __builtin_abort();
  a = 42;
}

int main() {
  int e = 1;
lab:
  if (a < 2) {
    int           b = e;
    _Bool         d = a != 0;
    _Bool         f = b != 0;
    unsigned long g = -(d & f);
    unsigned long h = c & g;
    unsigned long i = ~c;
    e               = -(i & h);
    c               = e != 0;
    a               = ~e + b;
    foo(e);
    goto lab;
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/119291
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     if x != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = 42 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<bool> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut bool = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut bool;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<bool> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut bool = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut bool;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_6]], 1 as i32);
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if (unsafe { a }) < (2 as i32) {
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_5]], *[[SLATE_SLOT_6]]);
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_4]], (unsafe { a }) != (0 as i32));
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_5]] != (0 as i32));
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_2]],
// CHECK-NEXT:                     (-((*[[SLATE_SLOT_4]] as i32) & (*[[SLATE_SLOT_3]] as i32)) as i64) as u64,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_1]], ((unsafe { c }) as u64) & *[[SLATE_SLOT_2]]);
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_0]], !unsafe { c } as u64);
// CHECK-NEXT:                 *[[SLATE_SLOT_6]] =
// CHECK-NEXT:                     ((*[[SLATE_SLOT_0]] & *[[SLATE_SLOT_1]]).wrapping_neg() as u32) as i32;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     c = (*[[SLATE_SLOT_6]] != (0 as i32)) as i64;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     a = !(*[[SLATE_SLOT_6]]) + *[[SLATE_SLOT_5]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 foo(*[[SLATE_SLOT_6]]);
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
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
