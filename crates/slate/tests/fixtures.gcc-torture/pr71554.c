/* PR target/71554 */

int v;

__attribute__((noinline, noclone)) void bar(void) { v++; }

__attribute__((noinline, noclone)) void foo(unsigned int x) {
  signed int y = ((-__INT_MAX__ - 1) / 2);
  signed int r;
  if (__builtin_mul_overflow(x, y, &r))
    bar();
}

int main() {
  foo(2);
  if (v)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/71554
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) {
// CHECK-NEXT:     let mut y: i32 = (-(2147483647 as i32) - (1 as i32)) / (2 as i32);
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = x as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = y as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as i32) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         bar();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo((2 as i32) as u32);
// CHECK-NEXT:     if (unsafe { v }) != (0 as i32) {
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
