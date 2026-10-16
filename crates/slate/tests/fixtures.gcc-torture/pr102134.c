/* PR tree-optimization/102134 */

typedef unsigned long long u64;

u64 g;

void foo(u64 a, u64 b, u64 c, u64 *r) {
  b     *= b;
  u64 x  = a && ((b >> (c & 63)) | ((b << (c & 63)) & g));
  *r     = x + a;
}

int main() {
  u64 x;
  foo(1, 3000, 0, &x);
  if (x != 2)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/102134
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: u64, mut b: u64, mut c: u64, mut r: &mut u64) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = b;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_mul(b);
// CHECK-NEXT:     b = [[V_1]];
// CHECK-NEXT:     let mut x: u64 = (a != (0 as u64)
// CHECK-NEXT:         && b >> (c & (((63 as i32) as i64) as u64))
// CHECK-NEXT:             | b << (c & (((63 as i32) as i64) as u64)) & unsafe { g }
// CHECK-NEXT:             != (0 as u64)) as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(r as *mut u64) = x.wrapping_add(a);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = 0 as u64;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         ((1 as i32) as i64) as u64,
// CHECK-NEXT:         ((3000 as i32) as i64) as u64,
// CHECK-NEXT:         ((0 as i32) as i64) as u64,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     if x != (((2 as i32) as i64) as u64) {
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
