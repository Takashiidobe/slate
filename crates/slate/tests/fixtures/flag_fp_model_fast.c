/* { dg-options "-ffp-model=fast" } */
int printf(const char *, ...);

int main(void) {
#ifdef __FAST_MATH__
  int fast = 1;
#else
  int fast = 0;
#endif
#ifdef __NO_MATH_ERRNO__
  int no_errno = 1;
#else
  int no_errno = 0;
#endif
#ifdef __FINITE_MATH_ONLY__
  int finite = __FINITE_MATH_ONLY__;
#else
  int finite = -1;
#endif
  printf("%d %d %d\n", fast, no_errno, finite);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-ffp-model=fast" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let fast: i32 = 0;
// CHECK-NEXT:     let no_errno: i32 = 1;
// CHECK-NEXT:     let finite: i32 = 0;
// CHECK-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), fast, no_errno, finite) };
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
