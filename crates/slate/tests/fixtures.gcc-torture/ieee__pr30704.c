/* { dg-do run }

   # Floating-point support is incomplete.
   { dg-skip-if "AVR doubles are floats" { avr-*-* } } */
/* PR middle-end/30704 */

typedef __SIZE_TYPE__ size_t;
extern void           abort(void);
extern int            memcmp(const void *, const void *, size_t);
extern void          *memcpy(void *, const void *, size_t);

long long f1(void) {
  long long t;
  double    d = 0x0.fffffffffffff000p-1022;
  memcpy(&t, &d, sizeof(long long));
  return t;
}

double f2(void) {
  long long t = 0x000fedcba9876543LL;
  double    d;
  memcpy(&d, &t, sizeof(long long));
  return d;
}

int main() {
  union {
    long long ll;
    double    d;
  } u;

  if (sizeof(long long) != sizeof(double) || __DBL_MIN_EXP__ != -1021)
    return 0;

  u.ll = f1();
  if (u.d != 0x0.fffffffffffff000p-1022)
    abort();

  u.d = f2();
  if (u.ll != 0x000fedcba9876543LL)
    abort();

  double    b = 234.0;
  long long c;
  double    d = b;
  memcpy(&c, &b, sizeof(double));
  long long e = c;
  if (memcmp(&e, &d, sizeof(double)) != 0)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # Floating-point support is incomplete.
// CHECK-NEXT: //! { dg-skip-if "AVR doubles are floats" { avr-*-* } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! PR middle-end/30704
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn memcpy([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     ll: i64,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1() -> i64 {
// CHECK-NEXT:     let mut t: i64 = 0 as i64;
// CHECK-NEXT:     let mut d: f64 = 2.225073858507201e-308f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(t) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(d) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2() -> f64 {
// CHECK-NEXT:     let mut t: i64 = 4483583629026627 as i64;
// CHECK-NEXT:     let mut d: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(d) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(t) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (8 as u64) != (8 as u64) || -(1021 as i32) != -(1021 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.ll = f1();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { u.d }) != 2.225073858507201e-308f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.d = f2();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { u.ll }) != (4483583629026627 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b: f64 = 234.0f64;
// CHECK-NEXT:     let mut c: i64 = 0 as i64;
// CHECK-NEXT:     let mut d: f64 = b;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(c) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(b) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut e: i64 = c;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(e) as *const (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(d) as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
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
