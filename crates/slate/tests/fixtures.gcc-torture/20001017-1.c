void abort(void);

void bug(double *Cref, char transb, int m, int n, int k, double a, double *A,
         int fdA, double *B, int fdB, double b, double *C, int fdC) {
  if (C != Cref)
    abort();
}

int main(void) {
  double A[1], B[1], C[1];

  bug(C, 'B', 1, 2, 3, 4.0, A, 5, B, 6, 7.0, C, 8);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn bug(
// CHECK-NEXT:     mut Cref: *mut f64,
// CHECK-NEXT:     mut transb: i8,
// CHECK-NEXT:     mut m: i32,
// CHECK-NEXT:     mut n: i32,
// CHECK-NEXT:     mut k: i32,
// CHECK-NEXT:     mut a: f64,
// CHECK-NEXT:     mut A: *mut f64,
// CHECK-NEXT:     mut fdA: i32,
// CHECK-NEXT:     mut B: *mut f64,
// CHECK-NEXT:     mut fdB: i32,
// CHECK-NEXT:     mut b: f64,
// CHECK-NEXT:     mut C: *mut f64,
// CHECK-NEXT:     mut fdC: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if C != Cref {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut A: [f64; 1] = [0 as f64; 1];
// CHECK-NEXT:     let mut B: [f64; 1] = [0 as f64; 1];
// CHECK-NEXT:     let mut C: [f64; 1] = [0 as f64; 1];
// CHECK-NEXT:     bug(
// CHECK-NEXT:         C.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         (66 as i32) as i8,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4.0f64,
// CHECK-NEXT:         A.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         B.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7.0f64,
// CHECK-NEXT:         C.as_mut_ptr() as *mut f64,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:     );
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
