extern void abort(void);
double      d = __FLT_MIN__ / 2.0;
int         main() {
  double x = __FLT_MIN__ / 2.0;
  if (x != d)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut d: f64 = (1.1754944e-38f32 as f64) / 2.0f64;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: f64 = (1.1754944e-38f32 as f64) / 2.0f64;
// CHECK-NEXT:     if x != unsafe { d } {
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
