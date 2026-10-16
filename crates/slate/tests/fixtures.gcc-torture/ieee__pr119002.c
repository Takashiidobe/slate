/* { dg-do run } */
/* PR rtl-optimization/119002 */

__attribute__((noipa)) unsigned int foo(void *x, float y, float z) {
  unsigned int a, b;
  float        c, d, e;
  c  = y;
  d  = z;
  a  = c < d;
  d  = y;
  e  = z;
  b  = d >= e;
  a |= b;
  return a;
}

int main() {
  if (foo((void *)0, 0.f, __builtin_nanf("")))
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! PR rtl-optimization/119002
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "nanf"]
// CHECK-NEXT:     fn __builtin_nanf([[V_0:__v[0-9]+]]: *const i8) -> f32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut (), mut y: f32, mut z: f32) -> u32 {
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut b: u32 = 0 as u32;
// CHECK-NEXT:     let mut c: f32 = y;
// CHECK-NEXT:     let mut d: f32 = z;
// CHECK-NEXT:     let mut e: f32 = 0 as f32;
// CHECK-NEXT:     a = (c < d) as u32;
// CHECK-NEXT:     d = y;
// CHECK-NEXT:     e = z;
// CHECK-NEXT:     b = (d >= e) as u32;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = a;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]] | b;
// CHECK-NEXT:     a = [[V_2]];
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(std::ptr::null_mut::<()>(), 0.0f32, unsafe {
// CHECK-NEXT:         __builtin_nanf(c"".as_ptr() as *const i8)
// CHECK-NEXT:     }) != (0 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
