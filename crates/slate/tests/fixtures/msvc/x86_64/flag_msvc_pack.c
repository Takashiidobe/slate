/* { dg-options "/Zc:preprocessor /Zp2" } */
#include <stddef.h>
void abort(void);

struct mixed {
  char c;
  int i;
  short h;
  double d;
};

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  struct mixed m = {1, 2, 3, 4.5};
  check(sizeof(struct mixed), 16);
  check(offsetof(struct mixed, i), 2);
  check(offsetof(struct mixed, d), 8);
  check(m.c, 1);
  check(m.i, 2);
  check(m.h, 3);
  check(m.d == 4.5, 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /Zp2" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mixed {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     h: i16,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check(got: i32, want: i32) {
// CHECK-NEXT:     if got != want {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let m: mixed = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         [[T_0]].i = 2i32;
// CHECK-NEXT:         [[T_0]].h = 3i16;
// CHECK-NEXT:         [[T_0]].d = 4.5f64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     check(16i32, 16i32);
// CHECK-NEXT:     check((((2usize as *mut i32) as u64) as u32) as i32, 2i32);
// CHECK-NEXT:     check((((8usize as *mut f64) as u64) as u32) as i32, 8i32);
// CHECK-NEXT:     check(m.c as i32, 1i32);
// CHECK-NEXT:     check(m.i, 2i32);
// CHECK-NEXT:     check(m.h as i32, 3i32);
// CHECK-NEXT:     check((m.d == 4.5f64) as i32, 1i32);
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
