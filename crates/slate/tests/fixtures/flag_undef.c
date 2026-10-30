/* { dg-options "-undef" } */
int printf(const char *, ...);

int main(void) {
#if defined(__linux__) || defined(linux) || defined(__x86_64__) || defined(__GNUC__) || defined(__clang__)
  int target_macros = 1;
#else
  int target_macros = 0;
#endif
#ifdef __STDC_VERSION__
  int stdc = 1;
#else
  int stdc = 0;
#endif
  printf("%d %d\n", target_macros, stdc);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-undef" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let target_macros: i32 = 0;
// CHECK-NEXT:     let stdc: i32 = 1;
// CHECK-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), target_macros, stdc) };
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
