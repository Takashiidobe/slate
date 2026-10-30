/* { dg-options "-maes -mpclmul -mrdrnd -msha -madx" } */
#include <stdio.h>

int main(void) {
#if defined(__AES__) && defined(__PCLMUL__) && defined(__RDRND__) && defined(__SHA__) && defined(__ADX__)
  int all = 1;
#else
  int all = 0;
#endif
#ifdef __AVX__
  int avx = 1;
#else
  int avx = 0;
#endif
  printf("%d %d\n", all, avx);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-maes -mpclmul -mrdrnd -msha -madx" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let all: i32 = 1;
// CHECK-NEXT:     let avx: i32 = 0;
// CHECK-NEXT:     println!("{} {}", all, avx);
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
