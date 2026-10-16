/* Note both PHI-OPT and the loop if conversion pass converts the inner if to be
 * branchless using min/max. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define RGBMAX 255

unsigned char test() {
  int            i, Pels;
  int            sum = 0;
  unsigned char  xr, xg, xb;
  unsigned char  xc, xm, xy, xk = 0;
  unsigned char *ReadPtr, *EritePtr;

  ReadPtr  = (unsigned char *)malloc(sizeof(unsigned char) * 100);
  EritePtr = (unsigned char *)malloc(sizeof(unsigned char) * 100);

  for (i = 0; i < 100; i++) {
    ReadPtr[i] = 100 - i;
  }

  for (i = 0; i < 24; i++) {
    xr = *ReadPtr++;
    xg = *ReadPtr++;
    xb = *ReadPtr++;

    xc = (unsigned char)(RGBMAX - xr);
    xm = (unsigned char)(RGBMAX - xg);
    xy = (unsigned char)(RGBMAX - xb);

    if (xc < xm) {
      xk = (unsigned char)(xc < xy ? xc : xy);
    } else {
      xk = (unsigned char)(xm < xy ? xm : xy);
    }

    xc = (unsigned char)(xc - xk);
    xm = (unsigned char)(xm - xk);
    xy = (unsigned char)(xy - xk);

    *EritePtr++  = xc;
    *EritePtr++  = xm;
    *EritePtr++  = xy;
    *EritePtr++  = xk;
    sum         += *(--EritePtr);
  }
  return sum;
}

int main() {
  if (test() != 196)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Note both PHI-OPT and the loop if conversion pass converts the inner if to be
// CHECK-NEXT: //! branchless using min/max.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test() -> u8 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut xr: u8 = 0 as u8;
// CHECK-NEXT:     let mut xg: u8 = 0 as u8;
// CHECK-NEXT:     let mut xb: u8 = 0 as u8;
// CHECK-NEXT:     let mut xc: u8 = 0 as u8;
// CHECK-NEXT:     let mut xm: u8 = 0 as u8;
// CHECK-NEXT:     let mut xy: u8 = 0 as u8;
// CHECK-NEXT:     let mut xk: u8 = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     let mut ReadPtr: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut EritePtr: *mut u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ReadPtr = (unsafe { malloc((1 as u64).wrapping_mul(((100 as i32) as i64) as u64)) }) as *mut u8;
// CHECK-NEXT:     EritePtr =
// CHECK-NEXT:         (unsafe { malloc((1 as u64).wrapping_mul(((100 as i32) as i64) as u64)) }) as *mut u8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (100 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { ReadPtr.offset(i as isize) } = (((100 as i32) - i) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (24 as i32) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut u8 = ReadPtr;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut u8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:         ReadPtr = [[V_3]];
// CHECK-NEXT:         xr = unsafe { *[[V_2]] };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut u8 = ReadPtr;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut u8 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:         ReadPtr = [[V_5]];
// CHECK-NEXT:         xg = unsafe { *[[V_4]] };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut u8 = ReadPtr;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut u8 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:         ReadPtr = [[V_7]];
// CHECK-NEXT:         xb = unsafe { *[[V_6]] };
// CHECK-NEXT:         xc = (((255 as i32) - ((xr as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         xm = (((255 as i32) - ((xg as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         xy = (((255 as i32) - ((xb as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         if ((xc as u32) as i32) < ((xm as u32) as i32) {
// CHECK-NEXT:             xk = ((if ((xc as u32) as i32) < ((xy as u32) as i32) {
// CHECK-NEXT:                 (xc as u32) as i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 (xy as u32) as i32
// CHECK-NEXT:             }) as i8) as u8;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             xk = ((if ((xm as u32) as i32) < ((xy as u32) as i32) {
// CHECK-NEXT:                 (xm as u32) as i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 (xy as u32) as i32
// CHECK-NEXT:             }) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         xc = ((((xc as u32) as i32) - ((xk as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         xm = ((((xm as u32) as i32) - ((xk as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         xy = ((((xy as u32) as i32) - ((xk as u32) as i32)) as i8) as u8;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut u8 = EritePtr;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut u8 = unsafe { [[V_8]].offset((1 as i32) as isize) };
// CHECK-NEXT:         EritePtr = [[V_9]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_8]] = xc;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: *mut u8 = EritePtr;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: *mut u8 = unsafe { [[V_10]].offset((1 as i32) as isize) };
// CHECK-NEXT:         EritePtr = [[V_11]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_10]] = xm;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: *mut u8 = EritePtr;
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: *mut u8 = unsafe { [[V_12]].offset((1 as i32) as isize) };
// CHECK-NEXT:         EritePtr = [[V_13]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_12]] = xy;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: *mut u8 = EritePtr;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: *mut u8 = unsafe { [[V_14]].offset((1 as i32) as isize) };
// CHECK-NEXT:         EritePtr = [[V_15]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_14]] = xk;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: *mut u8 = EritePtr;
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: *mut u8 = unsafe { [[V_17]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         EritePtr = [[V_18]];
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: i32 = [[V_16]] + (((unsafe { *[[V_18]] }) as u32) as i32);
// CHECK-NEXT:         sum = [[V_19]];
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (1 as i32);
// CHECK-NEXT:         i = [[V_21]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (sum as i8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((test() as u32) as i32) != (196 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
