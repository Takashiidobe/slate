/* { dg-do run } */
/* Copyright (C) 2003  Free Software Foundation.
   by Roger Sayle <roger@eyesopen.com>, derived from mzero3.c

   Constant folding of sin(-0.0), tan(-0.0) and atan(-0.0) should
   all return -0.0, for both double and float forms.  */

void                  abort(void);
typedef __SIZE_TYPE__ size_t;
extern int            memcmp(const void *, const void *, size_t);

double sin(double);
double tan(double);
double atan(double);

float sinf(float);
float tanf(float);
float atanf(float);

void expectd(double, double);
void expectf(float, float);

void expectd(double value, double expected) {
  if (value != expected ||
      memcmp((void *)&value, (void *)&expected, sizeof(double)) != 0)
    abort();
}

void expectf(float value, float expected) {
  if (value != expected ||
      memcmp((void *)&value, (void *)&expected, sizeof(float)) != 0)
    abort();
}

int main() {
  expectd(sin(0.0), 0.0);
  expectd(tan(0.0), 0.0);
  expectd(atan(0.0), 0.0);

  expectd(sin(-0.0), -0.0);
  expectd(tan(-0.0), -0.0);
  expectd(atan(-0.0), -0.0);

  expectf(sinf(0.0f), 0.0f);
  expectf(tanf(0.0f), 0.0f);
  expectf(atanf(0.0f), 0.0f);

  expectf(sinf(-0.0f), -0.0f);
  expectf(tanf(-0.0f), -0.0f);
  expectf(atanf(-0.0f), -0.0f);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Copyright (C) 2003  Free Software Foundation.
// CHECK-NEXT: //! by Roger Sayle <roger@eyesopen.com>, derived from mzero3.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! Constant folding of sin(-0.0), tan(-0.0) and atan(-0.0) should
// CHECK-NEXT: //! all return -0.0, for both double and float forms.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn sin([[V_3:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn tan([[V_4:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn atan([[V_5:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn sinf([[V_6:__v[0-9]+]]: f32) -> f32;
// CHECK-NEXT:     fn tanf([[V_7:__v[0-9]+]]: f32) -> f32;
// CHECK-NEXT:     fn atanf([[V_8:__v[0-9]+]]: f32) -> f32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn expectd(mut value: f64, mut expected: f64) {
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if value != expected {
// CHECK-NEXT:         [[V_9]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(value) as *mut ()) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(expected) as *mut ()) as *const (),
// CHECK-NEXT:                 8 as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_9]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn expectf(mut value: f32, mut expected: f32) {
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if value != expected {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(value) as *mut ()) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(expected) as *mut ()) as *const (),
// CHECK-NEXT:                 4 as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     expectd(unsafe { sin(0.0f64) }, 0.0f64);
// CHECK-NEXT:     expectd(unsafe { tan(0.0f64) }, 0.0f64);
// CHECK-NEXT:     expectd(unsafe { atan(0.0f64) }, 0.0f64);
// CHECK-NEXT:     expectd(unsafe { sin(-0.0f64) }, -0.0f64);
// CHECK-NEXT:     expectd(unsafe { tan(-0.0f64) }, -0.0f64);
// CHECK-NEXT:     expectd(unsafe { atan(-0.0f64) }, -0.0f64);
// CHECK-NEXT:     expectf(unsafe { sinf(0.0f32) }, 0.0f32);
// CHECK-NEXT:     expectf(unsafe { tanf(0.0f32) }, 0.0f32);
// CHECK-NEXT:     expectf(unsafe { atanf(0.0f32) }, 0.0f32);
// CHECK-NEXT:     expectf(unsafe { sinf(-0.0f32) }, -0.0f32);
// CHECK-NEXT:     expectf(unsafe { tanf(-0.0f32) }, -0.0f32);
// CHECK-NEXT:     expectf(unsafe { atanf(-0.0f32) }, -0.0f32);
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
