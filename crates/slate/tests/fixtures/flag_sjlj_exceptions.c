/* { dg-options "-fsjlj-exceptions" } */
int printf(const char *, ...);

int main(void) {
#ifdef __USING_SJLJ_EXCEPTIONS__
  int sjlj = __USING_SJLJ_EXCEPTIONS__;
#else
  int sjlj = 0;
#endif
  printf("%d\n", sjlj);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fsjlj-exceptions" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let sjlj: i32 = 1;
// CHECK-NEXT:     unsafe { printf(c"%d\n".as_ptr(), sjlj) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
