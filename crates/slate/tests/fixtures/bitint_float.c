#include <stdio.h>

#define CONV(NAME, T, UT)                                                     \
  static void NAME(T a, double d, float f) {                                  \
    printf("%.17g %.9g ", (double)a, (double)(float)a);                       \
    T x = (T)d; printf("%lld ", (long long)x);                                \
    T y = (T)f; printf("%lld ", (long long)y);                                \
    printf("%d%d\n", a == d, a < f);                                          \
  }

CONV(s5, _BitInt(5), 0)
CONV(s9, _BitInt(9), 0)
CONV(s33, _BitInt(33), 0)
CONV(s64, _BitInt(64), 0)
CONV(s100, _BitInt(100), 0)
CONV(s128, _BitInt(128), 0)
CONV(s200, _BitInt(200), 0)

static void wide(void) {
  _BitInt(200) big = 1;
  big <<= 150;
  printf("%.17g\n", (double)big);
  printf("%.9g\n", (double)(float)big);
  unsigned _BitInt(200) ub = 3;
  ub <<= 190;
  printf("%.17g\n", (double)ub);
  double d = 1.2345678901234567e40;
  _BitInt(200) fromd = (_BitInt(200))d;
  printf("%.17g\n", (double)fromd);
  double neg = -9.87654321e50;
  _BitInt(200) fromneg = (_BitInt(200))neg;
  printf("%.17g\n", (double)fromneg);
  unsigned _BitInt(200) fromu = (unsigned _BitInt(200))1e45;
  printf("%.17g\n", (double)fromu);
  _BitInt(200) rounding = 9007199254740993;
  printf("%.17g\n", (double)rounding);
  _BitInt(200) tie = (_BitInt(200))9007199254740993 * 1024 + 512;
  printf("%.17g\n", (double)tie);
  printf("%.9g\n", (double)(float)(_BitInt(100))16777217);
  printf("%d %d\n", (int)(_BitInt(200))-3.99, (int)(_BitInt(200))3.99);
}

int main(void) {
  s5(7, 3.9, -2.5f);
  s5(-16, -15.5, 14.9f);
  s9(255, -200.7, 100.3f);
  s33(4294967295LL, 123456789.9, -77.7f);
  s64(-9223372036854775807LL, 4e18, -4e18f);
  s100(123456789012345LL, 1e25, -1e25f);
  s128(-123456789012345LL, 1e30, -1e30f);
  s200(123456789012345LL, 1e40, -1e40f);
  wide();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn s5(a: i8, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a as f64, (a as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let x: i8 = (d as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let y: i8 = (f as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", ((a as f64) == d) as i32, ((a as f32) < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn s9(a: i16, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a as f64, (a as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let x: i16 = (d as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let y: i16 = (f as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", ((a as f64) == d) as i32, ((a as f32) < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn s33(a: i64, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a as f64, (a as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let x: i64 = (d as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let y: i64 = (f as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", ((a as f64) == d) as i32, ((a as f32) < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn s64(a: i64, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a as f64, (a as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let x: i64 = d as i64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let y: i64 = f as i64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", ((a as f64) == d) as i32, ((a as f32) < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn s100(mut a: bitint::BInt<100, 2, 16, bitint::A8>, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a.to_f64(), a.to_f32() as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut x: bitint::BInt<100, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f64(d);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut y: bitint::BInt<100, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f32(f);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", (a.to_f64() == d) as i32, (a.to_f32() < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn s128(mut a: bitint::BInt<128, 2, 16, bitint::A8>, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a.to_f64(), a.to_f32() as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut x: bitint::BInt<128, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f64(d);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut y: bitint::BInt<128, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f32(f);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", (a.to_f64() == d) as i32, (a.to_f32() < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn s200(mut a: bitint::BInt<200, 4, 32, bitint::A8>, d: f64, f: f32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.9g ".as_ptr(), a.to_f64(), a.to_f32() as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut x: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f64(d);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), x.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut y: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f32(f);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), y.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}{}", (a.to_f64() == d) as i32, (a.to_f32() < f) as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn wide() {
// CHECK-NEXT:     let mut big: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(1 as i128);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = big;
// CHECK-NEXT:     big = [[V_0]].shl_bits(150 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), big.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.9g\n".as_ptr(), big.to_f32() as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut ub: bitint::BUint<200, 4, 32, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(3 as i128));
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = ub;
// CHECK-NEXT:     ub = [[V_1]].shl_bits(190 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), ub.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let d: f64 = 1.2345678901234566e40f64;
// CHECK-NEXT:     let mut fromd: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f64(d);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), fromd.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let neg: f64 = -9.87654321e50f64;
// CHECK-NEXT:     let mut fromneg: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f64(neg);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), fromneg.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut fromu: bitint::BUint<200, 4, 32, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_f64(1e45f64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), fromu.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut rounding: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(9007199254740993 as i128);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), rounding.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut tie: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(9007199254740993 as i128)
// CHECK-NEXT:         .wrapping_mul(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1024 as i128))
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(512 as i128));
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g\n".as_ptr(), tie.to_f64()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.9g\n".as_ptr(),
// CHECK-NEXT:             <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(16777217 as i128).to_f32()
// CHECK-NEXT:                 as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", < bitint::BInt < 200, 4, 32, bitint::A8 >>::from_f64(- 3.99f64)
// CHECK-NEXT:         .to_i128() as i32, < bitint::BInt < 200, 4, 32, bitint::A8 >>::from_f64(3.99f64)
// CHECK-NEXT:         .to_i128() as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     s5(7i8, 3.9f64, -2.5f32);
// CHECK-NEXT:     s5(-16i8, -15.5f64, 14.9f32);
// CHECK-NEXT:     s9(255i16, -200.7f64, 100.3f32);
// CHECK-NEXT:     s33(4294967295i64, 123456789.9f64, -77.7f32);
// CHECK-NEXT:     s64(-9223372036854775807i64, 4e18f64, -4e18f32);
// CHECK-NEXT:     s100(
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(123456789012345 as i128),
// CHECK-NEXT:         1e25f64,
// CHECK-NEXT:         -1e25f32,
// CHECK-NEXT:     );
// CHECK-NEXT:     s128(
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-123456789012345 as i128),
// CHECK-NEXT:         1e30f64,
// CHECK-NEXT:         -1e30f32,
// CHECK-NEXT:     );
// CHECK-NEXT:     s200(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(123456789012345 as i128),
// CHECK-NEXT:         1e40f64,
// CHECK-NEXT:         -f32::INFINITY,
// CHECK-NEXT:     );
// CHECK-NEXT:     wide();
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
