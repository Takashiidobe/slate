/* Verify whether math functions are simplified.  */
/* { dg-require-effective-target c99_runtime } */
/* { dg-require-weak "" } */
void   abort(void);
double sin(double);
double floor(double);
float  t(float a) { return sin(a); }
float  q(float a) { return floor(a); }
double q1(float a) { return floor(a); }
int    main(void) {
#ifdef __OPTIMIZE__
  if (t(0) != 0)
    abort();
  if (q(0) != 0)
    abort();
  if (q1(0) != 0)
    abort();
#endif
  return 0;
}
__attribute__((weak)) double floor(double a) { abort(); }
__attribute__((weak)) float  floorf(float a) { return a; }
__attribute__((weak)) double sin(double a) { return a; }
__attribute__((weak)) float  sinf(float a) { abort(); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Verify whether math functions are simplified.
// CHECK-NEXT: //
// CHECK-NEXT: // { dg-require-effective-target c99_runtime }
// CHECK-NEXT: //
// CHECK-NEXT: // { dg-require-weak "" }
// CHECK-NEXT: fn t(mut a: f32) -> f32 {
// CHECK-NEXT:     sin(a as f64) as f32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn q(mut a: f32) -> f32 {
// CHECK-NEXT:     floor(a as f64) as f32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn q1(mut a: f32) -> f64 {
// CHECK-NEXT:     floor(a as f64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn floor(mut a: f64) -> f64 {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn floorf(mut a: f32) -> f32 {
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sin(mut a: f64) -> f64 {
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sinf(mut a: f32) -> f32 {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
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
