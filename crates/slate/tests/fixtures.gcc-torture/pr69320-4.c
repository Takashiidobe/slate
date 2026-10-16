#include <stdlib.h>

int   a;
char  b, d;
short c;
short fn1(int p1, int p2) { return p2 >= 2 ? p1 : p1 > p2; }

int main() {
  int *e = &a, *f = &a;
  b = 1;
  for (; b <= 9; b++) {
    c  = *e != 5 || d;
    *f = fn1(c || b, a);
  }
  if ((long long)a != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1(mut p1: i32, mut p2: i32) -> i16 {
// CHECK-NEXT:     (if p2 >= (2 as i32) { p1 } else { (p1 > p2) as i32 }) as i16
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-NEXT:     let mut f: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { b }) as i32) <= (9 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = ((unsafe { *e }) != (5 as i32) || (unsafe { d }) != (0 as i8)) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *f = fn1(
// CHECK-NEXT:                 ((unsafe { c }) != (0 as i16) || (unsafe { b }) != (0 as i8)) as i32,
// CHECK-NEXT:                 unsafe { a },
// CHECK-NEXT:             ) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i8 = unsafe { b };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i8 = (([[V_0]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { a }) as i64) != ((1 as i32) as i64) {
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
