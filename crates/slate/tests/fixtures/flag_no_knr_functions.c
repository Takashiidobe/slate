/* { dg-options "-std=c17 -fno-knr-functions" } */
int printf(const char *, ...);

int answer();

int main(void) {
  int prototyped = _Generic(&answer, int (*)(int): 0, int (*)(void): 1, default: 2);
  printf("%d %d\n", prototyped, answer());
  return 0;
}

int answer(void) { return 42; }

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
// CHECK-NEXT: // { dg-options "-std=c17 -fno-knr-functions" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let prototyped: i32 = 1;
// CHECK-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), prototyped, answer()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn answer() -> i32 {
// CHECK-NEXT:     42
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
