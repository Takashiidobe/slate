/* { dg-options "/Zc:preprocessor /DSLATE_A=3 /DSLATE_B#4 /DSLATE_C /DSLATE_D /USLATE_D" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  check(SLATE_A, 3);
  check(SLATE_B, 4);
  check(SLATE_C, 1);
#ifdef SLATE_D
  abort();
#endif
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
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /DSLATE_A=3 /DSLATE_B#4 /DSLATE_C /DSLATE_D /USLATE_D" }
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(3i32, 3i32);
// CHECK-NEXT:     check(4i32, 4i32);
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
