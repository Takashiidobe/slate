/* { dg-add-options stack_size } */

#include <stdio.h>

void abort(void);
void exit(int);

#ifndef STACK_SIZE
#define STACK_SIZE 200000
#endif

__inline__ static int dummy(int x) {
  int y;
  y = (long)(x * 4711.3);
  return y;
}

int getval(void);

int f2(double x) {
  unsigned short s;
  int            a, b, c, d, e, f, g, h, i, j;

  a = getval();
  b = getval();
  c = getval();
  d = getval();
  e = getval();
  f = getval();
  g = getval();
  h = getval();
  i = getval();
  j = getval();

  s = x;

  return a + b + c + d + e + f + g + h + i + j + s;
}

int x = 1;

int getval(void) { return x++; }

char buf[10];

void f() {
  char ar[STACK_SIZE / 2];
  int  a, b, c, d, e, f, g, h, i, j, k;

  a = getval();
  b = getval();
  c = getval();
  d = getval();
  e = getval();
  f = getval();
  g = getval();
  h = getval();
  i = getval();
  j = getval();

  k = f2(17.0);

  sprintf(buf, "%d\n", a + b + c + d + e + f + g + h + i + j + k);
  if (a + b + c + d + e + f + g + h + i + j + k != 227)
    abort();
}

int main(void) {
  f();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn sprintf(__s: *mut i8, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn dummy(mut [[X_0:x_[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = ((([[X_0]] as f64) * 4711.3f64) as i64) as i32;
// CHECK-NEXT:     y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut [[X_1:x_[0-9]+]]: f64) -> i32 {
// CHECK-NEXT:     let mut s: u16 = 0 as u16;
// CHECK-NEXT:     let mut a: i32 = getval();
// CHECK-NEXT:     let mut b: i32 = getval();
// CHECK-NEXT:     let mut c: i32 = getval();
// CHECK-NEXT:     let mut d: i32 = getval();
// CHECK-NEXT:     let mut e: i32 = getval();
// CHECK-NEXT:     let mut f: i32 = getval();
// CHECK-NEXT:     let mut g: i32 = getval();
// CHECK-NEXT:     let mut h: i32 = getval();
// CHECK-NEXT:     let mut i: i32 = getval();
// CHECK-NEXT:     let mut j: i32 = getval();
// CHECK-NEXT:     s = [[X_1]] as u16;
// CHECK-NEXT:     a + b + c + d + e + f + g + h + i + j + ((s as u32) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn getval() -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: [i8; 10] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     __SlateAlign16([0 as i8; 100000]);
// CHECK-NEXT:     let mut a: i32 = getval();
// CHECK-NEXT:     let mut b: i32 = getval();
// CHECK-NEXT:     let mut c: i32 = getval();
// CHECK-NEXT:     let mut d: i32 = getval();
// CHECK-NEXT:     let mut e: i32 = getval();
// CHECK-NEXT:     let mut f: i32 = getval();
// CHECK-NEXT:     let mut g: i32 = getval();
// CHECK-NEXT:     let mut h: i32 = getval();
// CHECK-NEXT:     let mut i: i32 = getval();
// CHECK-NEXT:     let mut j: i32 = getval();
// CHECK-NEXT:     let mut k: i32 = f2(17.0f64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sprintf(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(buf) as *mut i8 },
// CHECK-NEXT:             c"%d\n".as_ptr() as *const i8,
// CHECK-NEXT:             a + b + c + d + e + f + g + h + i + j + k,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if a + b + c + d + e + f + g + h + i + j + k != (227 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
