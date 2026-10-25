/* { dg-options "/Zc:preprocessor /std:c11" } */
void abort(void);

static void check(long got, long want) {
  if (got != want)
    abort();
}

struct aligned {
  _Alignas(16) char c;
};

_Static_assert(sizeof(int) == 4, "int");

int main(void) {
  check(__STDC_VERSION__, 201112L);
  check(_Alignof(struct aligned), 16);
  check(_Generic((char)0, char: 1, default: 0), 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /std:c11" }
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(201112i32, 201112i32);
// CHECK-NEXT:     check(16i32, 16i32);
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
