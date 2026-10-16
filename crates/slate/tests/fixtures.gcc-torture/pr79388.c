/* PR rtl-optimization/79388 */
/* { dg-additional-options "-fno-tree-coalesce-vars" } */

unsigned int a, c;

__attribute__((noinline, noclone)) unsigned int foo(unsigned int p) {
  p |= 1;
  p &= 0xfffe;
  p %= 0xffff;
  c  = p;
  return a + p;
}

int main(void) {
  int x = foo(6);
  if (x != 6)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/79388
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fno-tree-coalesce-vars" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: u32) -> u32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = p;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]] | ((1 as i32) as u32);
// CHECK-NEXT:     p = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = p;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]] & ((65534 as i32) as u32);
// CHECK-NEXT:     p = [[V_3]];
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = p;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]] % ((65535 as i32) as u32);
// CHECK-NEXT:     p = [[V_5]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = p;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { a }.wrapping_add(p)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = foo((6 as i32) as u32) as i32;
// CHECK-NEXT:     if x != (6 as i32) {
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
