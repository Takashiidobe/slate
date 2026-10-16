/* PR tree-optimization/85529 */

struct S {
  int a;
};

int               b, c = 1, d, e, f;
static int        g;
volatile struct S s;

signed char foo(signed char i, int j) { return i < 0 ? i : i << j; }

int main() {
  signed char k = -83;
  if (!d)
    goto L;
  k = e || f;
L:
  for (; b < 1; b++)
    s.a != (k < foo(k, 2) && (c = k = g));
  if (c != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/85529
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i8, mut j: i32) -> i8 {
// CHECK-NEXT:     (if (i as i32) < (0 as i32) {
// CHECK-NEXT:         i as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (i as i32) << j
// CHECK-NEXT:     }) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<bool> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut bool = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut bool;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_5]], -(83 as i32) as i8);
// CHECK-NEXT:             if !((unsafe { d }) != (0 as i32)) {
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 *[[SLATE_SLOT_5]] =
// CHECK-NEXT:                     ((unsafe { e }) != (0 as i32) || (unsafe { f }) != (0 as i32)) as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if (unsafe { b }) < (1 as i32) {
// CHECK-NEXT:                 if (*[[SLATE_SLOT_5]] as i32) < (foo(*[[SLATE_SLOT_5]], 2 as i32) as i32) {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_3]], (unsafe { g }) as i8);
// CHECK-NEXT:                     *[[SLATE_SLOT_5]] = *[[SLATE_SLOT_3]];
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_2]], *[[SLATE_SLOT_3]] as i32);
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         c = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     *[[SLATE_SLOT_4]] = *[[SLATE_SLOT_2]] != (0 as i32);
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     *[[SLATE_SLOT_4]] = false as bool;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_1]], unsafe { b });
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     b = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { c }) != (1 as i32) {
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
