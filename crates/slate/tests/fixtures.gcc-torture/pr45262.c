/* PR middle-end/45262 */

/* { dg-require-effective-target int32plus } */

extern void abort(void);

int foo(unsigned int x) { return ((int)x < 0) || ((int)(-x) < 0); }

int bar(unsigned int x) { return x >> 31 || (-x) >> 31; }

int main(void) {
  if (foo(1) != 1)
    abort();
  if (foo(0) != 0)
    abort();
  if (foo(-1) != 1)
    abort();
  if (bar(1) != 1)
    abort();
  if (bar(0) != 0)
    abort();
  if (bar(-1) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/45262
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: fn foo(mut x: u32) -> i32 {
// CHECK-NEXT:     ((x as i32) < (0 as i32) || (x.wrapping_neg() as i32) < (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u32) -> i32 {
// CHECK-NEXT:     (x >> (31 as i32) != (0 as u32) || x.wrapping_neg() >> (31 as i32) != (0 as u32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo((1 as i32) as u32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((0 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(-(1 as i32) as u32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar((1 as i32) as u32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar((0 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(-(1 as i32) as u32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
