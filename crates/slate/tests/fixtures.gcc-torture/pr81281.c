/* PR sanitizer/81281 */

void foo(unsigned p, unsigned a, unsigned b) {
  unsigned q = p + 7;
  if (a - (1U + __INT_MAX__) >= 2)
    __builtin_unreachable();
  int d = p + b;
  int c = p + a;
  if (c - d != __INT_MAX__)
    __builtin_abort();
}

void bar(unsigned p, unsigned a) {
  unsigned q = p + 7;
  if (a - (1U + __INT_MAX__) >= 2)
    __builtin_unreachable();
  int c = p;
  int d = p + a;
  if (c - d != -__INT_MAX__ - 1)
    __builtin_abort();
}

int main() {
  foo(-1U, 1U + __INT_MAX__, 1U);
  bar(-1U, 1U + __INT_MAX__);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR sanitizer/81281
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_unreachable();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: u32, mut a: u32, mut b: u32) {
// CHECK-NEXT:     p.wrapping_add((7 as i32) as u32);
// CHECK-NEXT:     if a.wrapping_sub((1 as u32).wrapping_add((2147483647 as i32) as u32)) >= ((2 as i32) as u32) {
// CHECK-NEXT:         unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut d: i32 = p.wrapping_add(b) as i32;
// CHECK-NEXT:     let mut c: i32 = p.wrapping_add(a) as i32;
// CHECK-NEXT:     if c - d != (2147483647 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: u32, mut a: u32) {
// CHECK-NEXT:     p.wrapping_add((7 as i32) as u32);
// CHECK-NEXT:     if a.wrapping_sub((1 as u32).wrapping_add((2147483647 as i32) as u32)) >= ((2 as i32) as u32) {
// CHECK-NEXT:         unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: i32 = p as i32;
// CHECK-NEXT:     let mut d: i32 = p.wrapping_add(a) as i32;
// CHECK-NEXT:     if c - d != -(2147483647 as i32) - (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(
// CHECK-NEXT:         (1 as u32).wrapping_neg(),
// CHECK-NEXT:         (1 as u32).wrapping_add((2147483647 as i32) as u32),
// CHECK-NEXT:         1 as u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     bar(
// CHECK-NEXT:         (1 as u32).wrapping_neg(),
// CHECK-NEXT:         (1 as u32).wrapping_add((2147483647 as i32) as u32),
// CHECK-NEXT:     );
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
