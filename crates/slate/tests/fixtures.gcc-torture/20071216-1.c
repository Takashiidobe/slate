/* PR rtl-optimization/34490 */

extern void abort(void);

static int x;

int __attribute__((noinline)) bar(void) { return x; }

int foo(void) {
  long int b = bar();
  if ((unsigned long)b < -4095L)
    return b;
  if (-b != 38)
    b = -2;
  return b + 1;
}

int main(void) {
  x = 26;
  if (foo() != 26)
    abort();
  x = -39;
  if (foo() != -1)
    abort();
  x = -38;
  if (foo() != -37)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/34490
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     unsafe { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut b: i64 = bar() as i64;
// CHECK-NEXT:     if (b as u64) < (-(4095 as i64) as u64) {
// CHECK-NEXT:         return b as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if -b != ((38 as i32) as i64) {
// CHECK-NEXT:         b = -(2 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     (b + ((1 as i32) as i64)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = 26 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != (26 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = -(39 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = -(38 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != -(37 as i32) {
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
