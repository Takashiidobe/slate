void abort(void);

int f(int a, int b) {
  if (a == 1)
    a = 0;
  if (b == 0)
    a = 1;
  if (a != 0)
    return 0;
  return 1;
}

int main(void) {
  if (f(1, 1) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     if a == (1 as i32) {
// CHECK-NEXT:         a = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if b == (0 as i32) {
// CHECK-NEXT:         a = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if a != (0 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(1 as i32, 1 as i32) != (1 as i32) {
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
