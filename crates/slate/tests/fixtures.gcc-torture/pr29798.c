extern void abort();

int main() {
  int    i;
  double oldrho;
  double beta = 0.0;
  double work = 1.0;
  for (i = 1; i <= 2; i++) {
    double rho = work * work;
    if (i != 1)
      beta = rho / oldrho;
    if (beta == 1.0)
      abort();

    /* All targets even remotely likely to ever get supported
       use at least an even base, so there will never be any
       floating-point rounding. All computation in this test
       case is exact for even bases.  */
    work   /= 2.0;
    oldrho  = rho;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     let mut oldrho: f64 = 0 as f64;
// CHECK-NEXT:     let mut beta: f64 = 0.0f64;
// CHECK-NEXT:     let mut work: f64 = 1.0f64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (2 as i32) {
// CHECK-NEXT:         let mut rho: f64 = work * work;
// CHECK-NEXT:         if i != (1 as i32) {
// CHECK-NEXT:             beta = rho / oldrho;
// CHECK-NEXT:         }
// CHECK-NEXT:         if beta == 1.0f64 {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         // All targets even remotely likely to ever get supported
// CHECK-NEXT:         // use at least an even base, so there will never be any
// CHECK-NEXT:         // floating-point rounding. All computation in this test
// CHECK-NEXT:         // case is exact for even bases.
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: f64 = work;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: f64 = [[V_0]] / 2.0f64;
// CHECK-NEXT:         work = [[V_1]];
// CHECK-NEXT:         oldrho = rho;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
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
