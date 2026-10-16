
#if (__SIZEOF_DOUBLE__ == 8)
double d = 1024.0 - 1.0 / 32768.0;
#else
double d = 1024.0 - 1.0 / 16384.0;
#endif

extern double floor(double);
extern float  floorf(float);
extern void   abort();

int main() {

  double df = floor(d);
  float  f1 = (float)floor(d);

  if ((int)df != 1023 || (int)f1 != 1023)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn floor([[V_0:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn floorf([[V_1:__v[0-9]+]]: f32) -> f32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut d: f64 = 1024.0f64 - 1.0f64 / 32768.0f64;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut df: f64 = unsafe { floor(unsafe { d }) };
// CHECK-NEXT:     let mut f1: f32 = (unsafe { floor(unsafe { d }) }) as f32;
// CHECK-NEXT:     if (df as i32) != (1023 as i32) || (f1 as i32) != (1023 as i32) {
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
