/* { dg-require-effective-target int32plus } */

#include <stdio.h>

void abort(void);
void exit(int);

typedef struct {
  int x;
  int y;
} S;
extern int foo(S *);
int        hi = 0, lo = 0;

int main() {
  S   a;
  int r;
  a.x = (int)0x00010000;
  a.y = 1;
  r   = foo(&a);
  if (r == 2 && lo == 0 && hi == 1) {
    exit(0);
  }
  abort();
}

typedef unsigned short u16;

__attribute__((noinline)) int foo(S *ptr) {
  int a = ptr->x;
  int c = 0;
  u16 b = (u16)a;
  if (b != 0) {
    lo  = 1;
    c  += ptr->y;
  }
  b = a >> 16;
  if (b != 0) {
    hi  = 1;
    c  += ptr->y;
  }
  c += ptr->y;
  return c;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut hi: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut lo: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     a.x = 65536 as i32;
// CHECK-NEXT:     a.y = 1 as i32;
// CHECK-NEXT:     r = foo(unsafe { &(*std::ptr::addr_of_mut!(a)) });
// CHECK-NEXT:     if r == (2 as i32) && (unsafe { lo }) == (0 as i32) && (unsafe { hi }) == (1 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut ptr: &[[SLATERECORD_0]]) -> i32 {
// CHECK-NEXT:     let mut a: i32 = unsafe { (*(ptr as *const [[SLATERECORD_0]])).x };
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: u16 = (a as i16) as u16;
// CHECK-NEXT:     if ((b as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             lo = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + unsafe { (*(ptr as *const [[SLATERECORD_0]])).y };
// CHECK-NEXT:         c = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     b = ((a >> (16 as i32)) as i16) as u16;
// CHECK-NEXT:     if ((b as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             hi = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + unsafe { (*(ptr as *const [[SLATERECORD_0]])).y };
// CHECK-NEXT:         c = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + unsafe { (*(ptr as *const [[SLATERECORD_0]])).y };
// CHECK-NEXT:     c = [[V_6]];
// CHECK-NEXT:     c
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
// SLATE-FILECHECK-END
