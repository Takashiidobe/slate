#include <stdarg.h>

void abort(void);
void exit(int);

double f(float f1, float f2, float f3, float f4, float f5, float f6, ...) {
  va_list ap;
  double  d;

  va_start(ap, f6);
  d = va_arg(ap, double);
  va_end(ap);
  return d;
}

int main() {
  if (f(1, 2, 3, 4, 5, 6, 7.0) != 7.0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn f(
// CHECK-NEXT:     mut f1: f32,
// CHECK-NEXT:     mut f2: f32,
// CHECK-NEXT:     mut f3: f32,
// CHECK-NEXT:     mut f4: f32,
// CHECK-NEXT:     mut f5: f32,
// CHECK-NEXT:     mut f6: f32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> f64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut d: f64 = 0 as f64;
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     d = unsafe { ap.next_arg::<f64>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         f(
// CHECK-NEXT:             (1 as i32) as f32,
// CHECK-NEXT:             (2 as i32) as f32,
// CHECK-NEXT:             (3 as i32) as f32,
// CHECK-NEXT:             (4 as i32) as f32,
// CHECK-NEXT:             (5 as i32) as f32,
// CHECK-NEXT:             (6 as i32) as f32,
// CHECK-NEXT:             7.0f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != 7.0f64
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
