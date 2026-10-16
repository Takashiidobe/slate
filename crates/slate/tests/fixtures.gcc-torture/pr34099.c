int foo(int b, int c) {
  int x;
  if (b)
    return x & c;
  else
    return 1;
}
extern void abort(void);
int         main() {
  if (foo(1, 0) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut b: i32, mut c: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         return x & c;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(1 as i32, 0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
