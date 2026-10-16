/* { dg-do run } */
/* Copyright (C) 2002  Free Software Foundation.
   by Hans-Peter Nilsson  <hp@bitrange.com>, derived from mzero2.c

   In the MMIX port, negdf2 was bogusly expanding -x into 0 - x.  */

void abort(void);
void exit(int);

double nzerod = -0.0;
float  nzerof = -0.0;
double zerod  = 0.0;
float  zerof  = 0.0;

void   expectd(double, double);
void   expectf(float, float);
double negd(double);
float  negf(float);

int main(void) {
  expectd(negd(zerod), nzerod);
  expectf(negf(zerof), nzerof);
  expectd(negd(nzerod), zerod);
  expectf(negf(nzerof), zerof);
  exit(0);
}

void expectd(double value, double expected) {
  if (value != expected ||
      __builtin_memcmp((void *)&value, (void *)&expected, sizeof(double)) != 0)
    abort();
}

void expectf(float value, float expected) {
  if (value != expected ||
      __builtin_memcmp((void *)&value, (void *)&expected, sizeof(float)) != 0)
    abort();
}

double negd(double v) { return -v; }

float negf(float v) { return -v; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //! by Hans-Peter Nilsson  <hp@bitrange.com>, derived from mzero2.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! In the MMIX port, negdf2 was bogusly expanding -x into 0 - x.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: *const (), [[V_3:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut nzerod: f64 = -0.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut nzerof: f32 = -0.0f64 as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut zerod: f64 = 0.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut zerof: f32 = 0.0f64 as f32;
// CHECK-EMPTY:
// CHECK-NEXT: fn expectd(mut value: f64, mut expected: f64) {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if value != expected {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(value) as *mut ()) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(expected) as *mut ()) as *const (),
// CHECK-NEXT:                 8 as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     expectd(negd(unsafe { zerod }), unsafe { nzerod });
// CHECK-NEXT:     expectf(negf(unsafe { zerof }), unsafe { nzerof });
// CHECK-NEXT:     expectd(negd(unsafe { nzerod }), unsafe { zerod });
// CHECK-NEXT:     expectf(negf(unsafe { nzerof }), unsafe { zerof });
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn expectf(mut value: f32, mut expected: f32) {
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if value != expected {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(value) as *mut ()) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(expected) as *mut ()) as *const (),
// CHECK-NEXT:                 4 as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn negd(mut v: f64) -> f64 {
// CHECK-NEXT:     -v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn negf(mut v: f32) -> f32 {
// CHECK-NEXT:     -v
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
