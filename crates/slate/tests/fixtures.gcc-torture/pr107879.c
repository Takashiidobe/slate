/* PR tree-optimization/107879 */

__attribute__((noipa)) static double foo(double *y) {
  volatile int    ph     = 0;
  volatile double vf     = 1.0;
  double          factor = vf;
  double          x      = -(double)ph * factor;
  if (x == 0)
    *y = 1.0;
  else
    *y = 1.0 / x;
  double w    = 2.0 * x / factor;
  double omww = 1 - w;
  return omww > 0.0 ? omww : 0.0;
}

int main() {
  double y = 42.0;
  if (foo(&y) != 1.0)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/107879
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut y: &mut f64) -> f64 {
// CHECK-NEXT:     let mut ph: i32 = 0 as i32;
// CHECK-NEXT:     let mut vf: f64 = 1.0f64;
// CHECK-NEXT:     let mut factor: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vf)) };
// CHECK-NEXT:     let mut x: f64 =
// CHECK-NEXT:         -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ph)) }) as f64) * factor;
// CHECK-NEXT:     if x == ((0 as i32) as f64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(y as *mut f64) = 1.0f64;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(y as *mut f64) = 1.0f64 / x;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut w: f64 = 2.0f64 * x / factor;
// CHECK-NEXT:     let mut omww: f64 = ((1 as i32) as f64) - w;
// CHECK-NEXT:     if omww > 0.0f64 { omww } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut y: f64 = 42.0f64;
// CHECK-NEXT:     if foo(unsafe { &mut (*std::ptr::addr_of_mut!(y)) }) != 1.0f64 {
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
