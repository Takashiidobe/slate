#include <stdio.h>

static _Float16 chain(_Float16 a, _Float16 b, _Float16 c) { return a * b + c; }
static _Float16 stored(_Float16 a, _Float16 b, _Float16 c) {
  _Float16 p = a * b;
  return p + c;
}
static _Float16 cast(_Float16 a, _Float16 b, _Float16 c) { return (_Float16)(a * b) + c; }
static _Float16 negated(_Float16 a, _Float16 b, _Float16 c) { return -(a * b) - c; }
static int compare(_Float16 a, _Float16 b, _Float16 c) { return a * b + c == 0; }
static _Float16 compound(_Float16 a, _Float16 b, _Float16 c) {
  c += a * b;
  return c;
}
static _Complex _Float16 cmul(_Complex _Float16 x, _Complex _Float16 y) { return x * y; }
static _Complex _Float16 cfma(_Complex _Float16 x, _Complex _Float16 y, _Complex _Float16 z) {
  return x * y + z;
}

int main(void) {
  _Float16 a = 1 + 0x1p-10f16;
  _Float16 c = -1 - 0x1p-9f16;
  printf("%a %a %a %a %d %a\n", (double)chain(a, a, c), (double)stored(a, a, c),
         (double)cast(a, a, c), (double)negated(a, a, c), compare(a, a, c),
         (double)compound(a, a, c));
  _Complex _Float16 x = a + a * 1.0if16;
  _Complex _Float16 y = a - a * 1.0if16;
  _Complex _Float16 z = c + c * 1.0if16;
  _Complex _Float16 p = cmul(x, y);
  _Complex _Float16 q = cfma(x, x, z);
  printf("%a %a %a %a\n", (double)__real__ p, (double)__imag__ p, (double)__real__ q,
         (double)__imag__ q);
  _Complex double w = 2.0;
  printf("%d %d %d %d\n", x * x == 0, a == x, w == 2.0, 3.0 != w);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f16)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn chain(a: f16, b: f16, c: f16) -> f16 {
// CHECK-NEXT:     ((a as f32) * (b as f32) + (c as f32)) as f16
// CHECK-NEXT: }
// CHECK-NEXT: fn stored(a: f16, b: f16, c: f16) -> f16 {
// CHECK-NEXT:     let p: f16 = ((a as f32) * (b as f32)) as f16;
// CHECK-NEXT:     ((p as f32) + (c as f32)) as f16
// CHECK-NEXT: }
// CHECK-NEXT: fn cast(a: f16, b: f16, c: f16) -> f16 {
// CHECK-NEXT:     (((((a as f32) * (b as f32)) as f16) as f32) + (c as f32)) as f16
// CHECK-NEXT: }
// CHECK-NEXT: fn negated(a: f16, b: f16, c: f16) -> f16 {
// CHECK-NEXT:     (-((a as f32) * (b as f32)) - (c as f32)) as f16
// CHECK-NEXT: }
// CHECK-NEXT: fn compare(a: f16, b: f16, c: f16) -> i32 {
// CHECK-NEXT:     ((((a as f32) * (b as f32) + (c as f32)) as f16) == ((0 as i32) as f16)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn compound(a: f16, b: f16, mut c: f16) -> f16 {
// CHECK-NEXT:     c = ((c as f32) + (a as f32) * (b as f32)) as f16;
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn cmul(
// CHECK-NEXT:     x: num_complex::Complex<f16>,
// CHECK-NEXT:     y: num_complex::Complex<f16>,
// CHECK-NEXT: ) -> num_complex::Complex<f16> {
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = x;
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]].re as f32,
// CHECK-NEXT:                 im: [[T_1]].im as f32,
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:             .c_mul(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_2:__t[0-9]+]] = y;
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_2]].re as f32,
// CHECK-NEXT:                         im: [[T_2]].im as f32,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 false,
// CHECK-NEXT:             );
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re as f16,
// CHECK-NEXT:             im: [[T_0]].im as f16,
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn cfma(
// CHECK-NEXT:     x: num_complex::Complex<f16>,
// CHECK-NEXT:     y: num_complex::Complex<f16>,
// CHECK-NEXT:     z: num_complex::Complex<f16>,
// CHECK-NEXT: ) -> num_complex::Complex<f16> {
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0]] = {
// CHECK-NEXT:             let [[T_1]] = x;
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]].re as f32,
// CHECK-NEXT:                 im: [[T_1]].im as f32,
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:             .c_mul(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_2]] = y;
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_2]].re as f32,
// CHECK-NEXT:                         im: [[T_2]].im as f32,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 false,
// CHECK-NEXT:             )
// CHECK-NEXT:             + {
// CHECK-NEXT:                 let [[T_3:__t[0-9]+]] = z;
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: [[T_3]].re as f32,
// CHECK-NEXT:                     im: [[T_3]].im as f32,
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re as f16,
// CHECK-NEXT:             im: [[T_0]].im as f16,
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: f16 = ((((1 as i32) as f16) as f32) + (9.7656E-4f16 as f32)) as f16;
// CHECK-NEXT:     let c: f16 = (((-(1 as i32) as f16) as f32) - (0.0019531f16 as f32)) as f16;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%a %a %a %a %d %a\n".as_ptr(),
// CHECK-NEXT:             chain(a, a, c) as f64,
// CHECK-NEXT:             stored(a, a, c) as f64,
// CHECK-NEXT:             cast(a, a, c) as f64,
// CHECK-NEXT:             negated(a, a, c) as f64,
// CHECK-NEXT:             compare(a, a, c),
// CHECK-NEXT:             compound(a, a, c) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let x: num_complex::Complex<f16> = {
// CHECK-NEXT:         let [[T_0]] = (a as f32)
// CHECK-NEXT:             + (a as f32)
// CHECK-NEXT:                 * {
// CHECK-NEXT:                     let [[T_1]] = num_complex::Complex {
// CHECK-NEXT:                         re: 0f16,
// CHECK-NEXT:                         im: 1f16,
// CHECK-NEXT:                     };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_1]].re as f32,
// CHECK-NEXT:                         im: [[T_1]].im as f32,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re as f16,
// CHECK-NEXT:             im: [[T_0]].im as f16,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let y: num_complex::Complex<f16> = {
// CHECK-NEXT:         let [[T_2]] = (a as f32)
// CHECK-NEXT:             - (a as f32)
// CHECK-NEXT:                 * {
// CHECK-NEXT:                     let [[T_3]] = num_complex::Complex {
// CHECK-NEXT:                         re: 0f16,
// CHECK-NEXT:                         im: 1f16,
// CHECK-NEXT:                     };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_3]].re as f32,
// CHECK-NEXT:                         im: [[T_3]].im as f32,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_2]].re as f16,
// CHECK-NEXT:             im: [[T_2]].im as f16,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let z: num_complex::Complex<f16> = {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = (c as f32)
// CHECK-NEXT:             + (c as f32)
// CHECK-NEXT:                 * {
// CHECK-NEXT:                     let [[T_5:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                         re: 0f16,
// CHECK-NEXT:                         im: 1f16,
// CHECK-NEXT:                     };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_5]].re as f32,
// CHECK-NEXT:                         im: [[T_5]].im as f32,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_4]].re as f16,
// CHECK-NEXT:             im: [[T_4]].im as f16,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let p: num_complex::Complex<f16> = cmul(x, y);
// CHECK-NEXT:     let q: num_complex::Complex<f16> = cfma(x, x, z);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%a %a %a %a\n".as_ptr(),
// CHECK-NEXT:             p.re as f64,
// CHECK-NEXT:             p.im as f64,
// CHECK-NEXT:             q.re as f64,
// CHECK-NEXT:             q.im as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let w: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 2.0f64,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", (({ let [[T_6:__t[0-9]+]] = { let [[T_7:__t[0-9]+]] = x; num_complex::Complex { re : [[T_7]].re
// CHECK-NEXT:         as f32, im : [[T_7]].im as f32 } } .c_mul({ let [[T_8:__t[0-9]+]] = x; num_complex::Complex { re
// CHECK-NEXT:         : [[T_8]].re as f32, im : [[T_8]].im as f32 } }, false); num_complex::Complex { re :
// CHECK-NEXT:         [[T_6]].re as f16, im : [[T_6]].im as f16 } }) == num_complex::Complex { re : (0 as
// CHECK-NEXT:         i32) as f16, im : 0.0 }) as i32, (num_complex::Complex { re : a, im : 0.0 } == x)
// CHECK-NEXT:         as i32, (w == num_complex::Complex { re : 2.0f64, im : 0.0 }) as i32,
// CHECK-NEXT:         (num_complex::Complex { re : 3.0f64, im : 0.0 } != w) as i32
// CHECK-NEXT:     );
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
