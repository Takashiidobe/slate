/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

void abort(void);
void exit(int);

#ifndef __vax__
static const unsigned long B1 = 715094163, /* B1 = (682-0.03306235651)*2**20 */
    B2                        = 696219795; /* B2 = (664-0.03306235651)*2**20 */

static const double
    C = 5.42857142857142815906e-01,  /* 19/35     = 0x3FE15F15, 0xF15F15F1 */
    D = -7.05306122448979611050e-01, /* -864/1225 = 0xBFE691DE, 0x2532C834 */
    E = 1.41428571428571436819e+00,  /* 99/70     = 0x3FF6A0EA, 0x0EA0EA0F */
    F = 1.60714285714285720630e+00,  /* 45/28     = 0x3FF9B6DB, 0x6DB6DB6E */
    G = 3.57142857142857150787e-01;  /* 5/14      = 0x3FD6DB6D, 0xB6DB6DB7 */

double cbrtl(double x) {
  long             hx;
  double           r, s, w;
  double           lt;
  unsigned         sign;
  typedef unsigned unsigned32 __attribute__((mode(SI)));
  union {
    double     t;
    unsigned32 pt[2];
  } ut, ux;
  int n0;

  ut.t = 1.0;
  n0   = (ut.pt[0] == 0);

  ut.t = 0.0;
  ux.t = x;

  hx    = ux.pt[n0];       /* high word of x */
  sign  = hx & 0x80000000; /* sign= sign(x) */
  hx   ^= sign;
  if (hx >= 0x7ff00000)
    return (x + x); /* cbrt(NaN,INF) is itself */
  if ((hx | ux.pt[1 - n0]) == 0)
    return (ux.t); /* cbrt(0) is itself */

  ux.pt[n0] = hx;
  /* rough cbrt to 5 bits */
  if (hx < 0x00100000) /* subnormal number */
  {
    ut.pt[n0]  = 0x43500000; /* set t= 2**54 */
    ut.t      *= x;
    ut.pt[n0]  = ut.pt[n0] / 3 + B2;
  } else
    ut.pt[n0] = hx / 3 + B1;

  /* new cbrt to 23 bits, may be implemented in single precision */
  r     = ut.t * ut.t / ux.t;
  s     = C + r * ut.t;
  ut.t *= G + F / (s + E + D / s);

  /* chopped to 20 bits and make it larger than cbrt(x) */
  ut.pt[1 - n0]  = 0;
  ut.pt[n0]     += 0x00000001;

  /* one step newton iteration to 53 bits with error less than 0.667 ulps */
  s    = ut.t * ut.t; /* t*t is exact */
  r    = ux.t / s;
  w    = ut.t + ut.t;
  r    = (r - ut.t) / (w + r); /* r-s is exact */
  ut.t = ut.t + ut.t * r;

  /* restore the sign bit */
  ut.pt[n0] |= sign;

  lt  = ut.t;
  lt -= (lt - (x / (lt * lt))) * 0.333333333333333333333;
  return lt;
}

int main(void) {
  if ((int)(cbrtl(27.0) + 0.5) != 3)
    abort();

  exit(0);
}
#else
int main(void) { exit(0); }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Developed at SunPro, a Sun Microsystems, Inc. business.
// CHECK-NEXT: //! Permission to use, copy, modify, and distribute this
// CHECK-NEXT: //! software is freely granted, provided that this notice
// CHECK-NEXT: //! is preserved.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     t: f64,
// CHECK-NEXT:     pt: [u32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut B1: u64 = ((715094163 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: /// B1 = (682-0.03306235651)*2**20
// CHECK-NEXT: /// B2 = (664-0.03306235651)*2**20
// CHECK-NEXT: static mut B2: u64 = ((696219795 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut C: f64 = 0.5428571428571428f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut D: f64 = -0.7053061224489796f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut E: f64 = 1.4142857142857144f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut F: f64 = 1.6071428571428572f64;
// CHECK-EMPTY:
// CHECK-NEXT: /// 19/35     = 0x3FE15F15, 0xF15F15F1
// CHECK-NEXT: /// -864/1225 = 0xBFE691DE, 0x2532C834
// CHECK-NEXT: /// 99/70     = 0x3FF6A0EA, 0x0EA0EA0F
// CHECK-NEXT: /// 45/28     = 0x3FF9B6DB, 0x6DB6DB6E
// CHECK-NEXT: /// 5/14      = 0x3FD6DB6D, 0xB6DB6DB7
// CHECK-NEXT: static mut G: f64 = 0.35714285714285715f64;
// CHECK-EMPTY:
// CHECK-NEXT: fn cbrtl(mut x: f64) -> f64 {
// CHECK-NEXT:     let mut hx: i64 = 0 as i64;
// CHECK-NEXT:     let mut r: f64 = 0 as f64;
// CHECK-NEXT:     let mut s: f64 = 0 as f64;
// CHECK-NEXT:     let mut w: f64 = 0 as f64;
// CHECK-NEXT:     let mut lt: f64 = 0 as f64;
// CHECK-NEXT:     let mut sign: u32 = 0 as u32;
// CHECK-NEXT:     let mut ut: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ux: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n0: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ut.t = 1.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     n0 = ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) == ((0 as i32) as u32)) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ut.t = 0.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ux.t = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     hx = ((unsafe { *unsafe { unsafe { ux.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) } })
// CHECK-NEXT:         as u64) as i64; // high word of x
// CHECK-NEXT:     sign = ((hx & (((2147483648 as u32) as u64) as i64)) as i32) as u32; // sign= sign(x)
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = hx;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] ^ ((sign as u64) as i64);
// CHECK-NEXT:     hx = [[V_2]];
// CHECK-NEXT:     if hx >= ((2146435072 as i32) as i64) {
// CHECK-NEXT:         return x + x;
// CHECK-NEXT:     }
// CHECK-NEXT:     // cbrt(NaN,INF) is itself
// CHECK-NEXT:     if hx
// CHECK-NEXT:         | (((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { ux.pt.as_mut_ptr() as *mut u32 }.offset(((1 as i32) - n0) as isize) }
// CHECK-NEXT:         }) as u64) as i64)
// CHECK-NEXT:         == ((0 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return unsafe { ux.t };
// CHECK-NEXT:     }
// CHECK-NEXT:     // cbrt(0) is itself
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { ux.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) } =
// CHECK-NEXT:             (hx as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     // rough cbrt to 5 bits
// CHECK-NEXT:     if hx < ((1048576 as i32) as i64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) } =
// CHECK-NEXT:                 (1129316352 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         // set t= 2**54
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: f64 = unsafe { ut.t };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: f64 = [[V_3]] * x;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ut.t = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) } =
// CHECK-NEXT:                 (((unsafe {
// CHECK-NEXT:                     *unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) }
// CHECK-NEXT:                 }) / ((3 as i32) as u32)) as u64)
// CHECK-NEXT:                     .wrapping_add(unsafe { B2 }) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) } =
// CHECK-NEXT:                 ((hx / ((3 as i32) as i64)) as u64).wrapping_add(unsafe { B1 }) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     // subnormal number
// CHECK-NEXT:     // new cbrt to 23 bits, may be implemented in single precision
// CHECK-NEXT:     r = (unsafe { ut.t }) * unsafe { ut.t } / unsafe { ux.t };
// CHECK-NEXT:     s = (unsafe { C }) + r * unsafe { ut.t };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: f64 = unsafe { ut.t };
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: f64 =
// CHECK-NEXT:         [[V_5]] * ((unsafe { G }) + (unsafe { F }) / (s + unsafe { E } + (unsafe { D }) / s));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ut.t = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // chopped to 20 bits and make it larger than cbrt(x)
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(((1 as i32) - n0) as isize) } =
// CHECK-NEXT:             (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: *mut u32 =
// CHECK-NEXT:         unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u32 = unsafe { *[[V_7]] };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u32 = [[V_8]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_7]] = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // one step newton iteration to 53 bits with error less than 0.667 ulps
// CHECK-NEXT:     s = (unsafe { ut.t }) * unsafe { ut.t }; // t*t is exact
// CHECK-NEXT:     r = (unsafe { ux.t }) / s;
// CHECK-NEXT:     w = (unsafe { ut.t }) + unsafe { ut.t };
// CHECK-NEXT:     r = (r - unsafe { ut.t }) / (w + r); // r-s is exact
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ut.t = (unsafe { ut.t }) + (unsafe { ut.t }) * r;
// CHECK-NEXT:     }
// CHECK-NEXT:     // restore the sign bit
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: *mut u32 =
// CHECK-NEXT:         unsafe { unsafe { ut.pt.as_mut_ptr() as *mut u32 }.offset(n0 as isize) };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u32 = unsafe { *[[V_10]] };
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u32 = [[V_11]] | sign;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_10]] = [[V_12]];
// CHECK-NEXT:     }
// CHECK-NEXT:     lt = unsafe { ut.t };
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: f64 = lt;
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: f64 = [[V_13]] - (lt - x / (lt * lt)) * 0.3333333333333333f64;
// CHECK-NEXT:     lt = [[V_14]];
// CHECK-NEXT:     lt
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((cbrtl(27.0f64) + 0.5f64) as i32) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
