/* { dg-additional-options "-fwrapv" } */
void abort(void);
void exit(int);

int errflag;

long long f(long long x, long long y) {
  long long r;

  errflag = 0;
  r       = x + y;
  if (x >= 0) {
    if ((y < 0) || (r >= 0))
      return r;
  } else {
    if ((y > 0) || (r < 0))
      return r;
  }
  errflag = 1;
  return 0;
}

int main(void) {
  f(0, 0);
  if (errflag)
    abort();

  f(1, -1);
  if (errflag)
    abort();

  f(-1, 1);
  if (errflag)
    abort();

  f(0x8000000000000000LL, 0x8000000000000000LL);
  if (!errflag)
    abort();

  f(0x8000000000000000LL, -1LL);
  if (!errflag)
    abort();

  f(0x7fffffffffffffffLL, 0x7fffffffffffffffLL);
  if (!errflag)
    abort();

  f(0x7fffffffffffffffLL, 1LL);
  if (!errflag)
    abort();

  f(0x7fffffffffffffffLL, 0x8000000000000000LL);
  if (errflag)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-additional-options "-fwrapv" }
// CHECK-NEXT: static mut errflag: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut x: i64, mut y: i64) -> i64 {
// CHECK-NEXT:     let mut r: i64 = 0 as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         errflag = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = x.wrapping_add(y);
// CHECK-NEXT:     if x >= ((0 as i32) as i64) {
// CHECK-NEXT:         if y < ((0 as i32) as i64) || r >= ((0 as i32) as i64) {
// CHECK-NEXT:             return r;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if y > ((0 as i32) as i64) || r < ((0 as i32) as i64) {
// CHECK-NEXT:             return r;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         errflag = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     (0 as i32) as i64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f((0 as i32) as i64, (0 as i32) as i64);
// CHECK-NEXT:     if (unsafe { errflag }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f((1 as i32) as i64, (1 as i32).wrapping_neg() as i64);
// CHECK-NEXT:     if (unsafe { errflag }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f((1 as i32).wrapping_neg() as i64, (1 as i32) as i64);
// CHECK-NEXT:     if (unsafe { errflag }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(
// CHECK-NEXT:         (9223372036854775808 as u64) as i64,
// CHECK-NEXT:         (9223372036854775808 as u64) as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     if !((unsafe { errflag }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(
// CHECK-NEXT:         (9223372036854775808 as u64) as i64,
// CHECK-NEXT:         (1 as i64).wrapping_neg(),
// CHECK-NEXT:     );
// CHECK-NEXT:     if !((unsafe { errflag }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(9223372036854775807 as i64, 9223372036854775807 as i64);
// CHECK-NEXT:     if !((unsafe { errflag }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(9223372036854775807 as i64, 1 as i64);
// CHECK-NEXT:     if !((unsafe { errflag }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(
// CHECK-NEXT:         9223372036854775807 as i64,
// CHECK-NEXT:         (9223372036854775808 as u64) as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe { errflag }) != (0 as i32) {
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
