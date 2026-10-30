// { dg-options "-std=c23" }
#include <stdio.h>

struct pair {
  int a;
  float b;
  int c[3];
};

constexpr struct pair base = {2, 3.5f, {4, 5, 6}};
constexpr struct pair alias = base;
static struct pair copied = alias;
struct pair exported = base;

int main(void) {
  copied.a += 10;
  printf("%d %g %d %d %d\n", copied.a, copied.b, copied.c[0], copied.c[1], copied.c[2]);
  printf("%d %g %d\n", exported.a, exported.b, exported.c[2]);
  printf("%d %d\n", alias.a, base.c[1]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(long_running_const_eval)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=c23" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: f32,
// CHECK-NEXT:     c: [i32; 3],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut base: pair = pair {
// CHECK-NEXT:     a: 2i32,
// CHECK-NEXT:     b: 3.5f32,
// CHECK-NEXT:     c: [4i32, 5i32, 6i32],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut alias: pair = pair {
// CHECK-NEXT:     a: 2i32,
// CHECK-NEXT:     b: 3.5f32,
// CHECK-NEXT:     c: [4i32, 5i32, 6i32],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut copied: pair = pair {
// CHECK-NEXT:     a: 2i32,
// CHECK-NEXT:     b: 3.5f32,
// CHECK-NEXT:     c: [4i32, 5i32, 6i32],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut exported: pair = pair {
// CHECK-NEXT:     a: 2i32,
// CHECK-NEXT:     b: 3.5f32,
// CHECK-NEXT:     c: [4i32, 5i32, 6i32],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         copied.a = copied.a + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %g %d %d %d\n".as_ptr(),
// CHECK-NEXT:             copied.a,
// CHECK-NEXT:             copied.b as f64,
// CHECK-NEXT:             copied.c[0],
// CHECK-NEXT:             copied.c[1],
// CHECK-NEXT:             copied.c[2],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %g %d\n".as_ptr(),
// CHECK-NEXT:             exported.a,
// CHECK-NEXT:             exported.b as f64,
// CHECK-NEXT:             exported.c[2],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", unsafe { alias.a }, unsafe { base.c[1] });
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
