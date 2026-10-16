#include <stdbool.h>

void abort(void);
void exit(int);

int f(int a, int b, int c, _Bool d, _Bool e, _Bool f, char g) {
  if (g != 1 || d != true || e != true || f != true)
    abort();
  return a + b + c;
}

int main(void) {
  if (f(1, 2, -3, true, true, true, '\001'))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: bool,
// CHECK-NEXT:     mut e: bool,
// CHECK-NEXT:     mut f: bool,
// CHECK-NEXT:     mut g: i8,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     if (g as i32) != (1 as i32) || (d as i32) != ((true as bool) as i32)
// CHECK-NEXT:         || (e as i32) != ((true as bool) as i32) || (f as i32) != ((true as bool) as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     a + b + c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         -(3 as i32),
// CHECK-NEXT:         true as bool,
// CHECK-NEXT:         true as bool,
// CHECK-NEXT:         true as bool,
// CHECK-NEXT:         (1 as i32) as i8,
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
