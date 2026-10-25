/* { dg-options "/Zc:preprocessor /std:clatest" } */
void abort(void);

static void check(long got, long want) {
  if (got != want)
    abort();
}

int main(void) {
  int value = 7;
  typeof(value) copy = value + 1;
  check(__STDC_VERSION__, 202312L);
  check(copy, 8);
  check(sizeof(typeof(char)), 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /std:clatest" }
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let value: i32 = 7;
// CHECK-NEXT:     let copy: i32 = value + 1;
// CHECK-NEXT:     check(202312i32, 202312i32);
// CHECK-NEXT:     check(copy, 8i32);
// CHECK-NEXT:     check(1i32, 1i32);
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
