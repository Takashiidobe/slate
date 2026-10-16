#include <stdlib.h>
int         a, b, d, f;
char        c;
static int *e = &d;
int         main() {
  int g = -1L;
  *e    = g;
  c     = 4;
  for (; c >= 14; c++)
    *e = 1;
  f      = a == 0;
  *e    ^= f;
  int h  = ~d;
  if (d)
    b = h;
  if (h)
    exit(0);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: *mut i32 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut g: i32 = -(1 as i64) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { e } = g;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = (4 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { c }) as i32) >= (14 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { e } = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i8 = unsafe { c };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i8 = (([[V_0]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = ((unsafe { a }) == (0 as i32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i32 = unsafe { e };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = unsafe { *[[V_2]] };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] ^ unsafe { f };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut h: i32 = !unsafe { d };
// CHECK-NEXT:     if (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = h;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if h != (0 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
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
