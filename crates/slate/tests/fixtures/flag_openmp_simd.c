/* { dg-options "-fopenmp-simd" } */
int printf(const char *, ...);

int main(void) {
  int values[64];
  for (int i = 0; i < 64; i++)
    values[i] = i * 3;
  long sum = 0;
#pragma omp simd reduction(+ : sum)
  for (int i = 0; i < 64; i++)
    sum += values[i];
#ifdef _OPENMP
  int openmp = 1;
#else
  int openmp = 0;
#endif
  printf("%ld %d\n", sum, openmp);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// X86_64-UNKNOWN-LINUX-GNU-NEXT: struct __SlateAlign16<T>(T);
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// CHECK-NEXT: // { dg-options "-fopenmp-simd" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let mut values: __SlateAlign16<[i32; 64]> = __SlateAlign16([0i32; 64]);
// I686-UNKNOWN-LINUX-GNU-NEXT:     let mut values: [i32; 64] = [0i32; 64];
// CHECK-NEXT:     for i in 0i32..64i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         values.0[i as usize] = i * 3;
// I686-UNKNOWN-LINUX-GNU-NEXT:         values[i as usize] = i * 3;
// CHECK-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let mut sum: i64 = 0;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     for i in values.0.iter().copied() {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         sum += i as i64;
// I686-UNKNOWN-LINUX-GNU-NEXT:     let mut sum: i32 = 0;
// I686-UNKNOWN-LINUX-GNU-NEXT:     for i in values.iter().copied() {
// I686-UNKNOWN-LINUX-GNU-NEXT:         sum += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let openmp: i32 = 0;
// CHECK-NEXT:     unsafe { printf(c"%ld %d\n".as_ptr(), sum, openmp) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
