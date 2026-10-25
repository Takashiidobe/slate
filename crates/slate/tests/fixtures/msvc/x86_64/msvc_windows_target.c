/* { dg-options "/Zc:preprocessor" } */
void abort(void);

static void check(long long got, long long want) {
  if (got != want)
    abort();
}

struct mixed {
  char c;
  long l;
  long long ll;
};

int main(void) {
#if !defined(_MSC_VER) || !defined(_WIN64) || !defined(_M_X64) || _MSVC_TRADITIONAL
  abort();
#endif
  check(sizeof(long), 4);
  check(sizeof(void *), 8);
  check(sizeof(L'a'), 2);
  check(sizeof(struct mixed), 16);
  check(sizeof(long double), 8);
  return 3;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: fn check(got: i64, want: i64) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(4i64, 4i64);
// CHECK-NEXT:     check(8i64, 8i64);
// CHECK-NEXT:     check(2i64, 2i64);
// CHECK-NEXT:     check(16i64, 16i64);
// CHECK-NEXT:     check(8i64, 8i64);
// CHECK-NEXT:     3
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
