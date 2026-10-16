/* PR tree-optimization/118915 */

int a;

int foo(int c, int d, int e, int f) {
  if (!d || !e)
    return -22;
  if (c > 16)
    return -22;
  if (!f)
    return -22;
  return 2;
}

int main() {
  if (foo(a + 21, a + 6, a + 34, a + 26) != -22)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/118915
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut c: i32, mut d: i32, mut e: i32, mut f: i32) -> i32 {
// CHECK-NEXT:     if !(d != (0 as i32)) || !(e != (0 as i32)) {
// CHECK-NEXT:         return -(22 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if c > (16 as i32) {
// CHECK-NEXT:         return -(22 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(f != (0 as i32)) {
// CHECK-NEXT:         return -(22 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     2 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         (unsafe { a }) + (21 as i32),
// CHECK-NEXT:         (unsafe { a }) + (6 as i32),
// CHECK-NEXT:         (unsafe { a }) + (34 as i32),
// CHECK-NEXT:         (unsafe { a }) + (26 as i32),
// CHECK-NEXT:     ) != -(22 as i32)
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
