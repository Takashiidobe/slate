#include <limits.h>
#include <stdio.h>

static double half(void) { return 0.5; }

int main(void) {
  double values[] = {3.9, -3.9, 0.999, -0.999, 255.5, 2147483647.0, -2147483648.0};
  for (int i = 0; i < 7; i++)
    printf("%d ", (int)values[i]);
  printf("\n");

  printf("%u %u\n", (unsigned)3.9, (unsigned)4294967295.0);
  printf("%u %d\n", (unsigned)(unsigned char)200.7, (int)(signed char)-100.5);
  printf("%d %d\n", (short)-32768.9, (unsigned short)65535.9);
  printf("%lld %lld\n", (long long)1e18, (long long)-9.2e18);
  printf("%llu\n", (unsigned long long)1.8e19);

  printf("%.1f %.1f\n", (double)LLONG_MAX, (double)ULLONG_MAX);
  printf("%.1f %.1f\n", (float)INT_MAX, (float)16777217);
  printf("%.1f %.1f\n", (double)4294967295u, (double)INT_MIN);
  unsigned long long big = 0xfffffffffffff801ULL;
  printf("%.1f %.1f\n", (double)big, (float)big);
  signed char sc = -7;
  unsigned char uc = 250;
  printf("%g %g\n", (double)sc, (float)uc);

  double precise = 0.1;
  float narrowed = (float)precise;
  double widened = narrowed;
  printf("%.17g %.17g\n", (double)narrowed, widened);
  printf("%.9g\n", (float)1e-50);
  printf("%g\n", (double)(float)1e39);

  _Bool from_half = half();
  _Bool from_tiny = 1e-300;
  _Bool from_zero = -0.0;
  _Bool from_nan  = 0.0 / (half() - 0.5);
  printf("%d %d %d %d\n", from_half, from_tiny, from_zero, from_nan);

  int n   = 10;
  double r = n / 4;
  float s  = n / 4.0f;
  long l   = 3.99f;
  printf("%g %g %ld\n", r, s, l);
  n = 2.75;
  n += 1.5;
  printf("%d\n", n);
  unsigned u = 7;
  u *= 0.5;
  printf("%u\n", u);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn half() -> f64 {
// CHECK-NEXT:     0.5f64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: __SlateAlign16<[f64; 7]> = __SlateAlign16([
// CHECK-NEXT:         3.9f64,
// CHECK-NEXT:         -3.9f64,
// CHECK-NEXT:         0.999f64,
// CHECK-NEXT:         -0.999f64,
// CHECK-NEXT:         255.5f64,
// CHECK-NEXT:         2147483647.0f64,
// CHECK-NEXT:         -2147483648.0f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0i32..7i32 {
// CHECK-NEXT:         print!("{} ", values.0[i as usize] as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     unsafe { printf(c"%u %u\n".as_ptr(), 3.9f64 as u32, 4294967295.0f64 as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%u %d\n".as_ptr(), (200.7f64 as u8) as u32, (-100.5f64 as i8) as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", (- 32768.9f64 as i16) as i32, ((65535.9f64 as u16) as u32) as i32);
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), 1e18f64 as i64, -9.2e18f64 as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), 1.8e19f64 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.1f %.1f\n".as_ptr(),
// CHECK-NEXT:             9223372036854775807i64 as f64,
// CHECK-NEXT:             18446744073709551615u64 as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.1f %.1f\n".as_ptr(),
// CHECK-NEXT:             (2147483647i32 as f32) as f64,
// CHECK-NEXT:             (16777217i32 as f32) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%.1f %.1f\n".as_ptr(), 4294967295u32 as f64, -2147483648i32 as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let big: u64 = 18446744073709549569;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.1f %.1f\n".as_ptr(), big as f64, (big as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let sc: i8 = -7;
// CHECK-NEXT:     let uc: u8 = 250u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g\n".as_ptr(), sc as f64, (uc as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let precise: f64 = 0.1f64;
// CHECK-NEXT:     let narrowed: f32 = precise as f32;
// CHECK-NEXT:     let widened: f64 = narrowed as f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %.17g\n".as_ptr(), narrowed as f64, widened) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.9g\n".as_ptr(), (1e-50f64 as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g\n".as_ptr(), (1e39f64 as f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let from_half: bool = half() != 0.0f64;
// CHECK-NEXT:     let from_tiny: bool = 1e-300f64 != 0.0f64;
// CHECK-NEXT:     let from_zero: bool = -0.0f64 != 0.0f64;
// CHECK-NEXT:     let from_nan: bool = 0.0f64 / (half() - 0.5f64) != 0.0f64;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", from_half as i32, from_tiny as i32, from_zero as i32, from_nan as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut n: i32 = 10;
// CHECK-NEXT:     let r: f64 = (n / 4) as f64;
// CHECK-NEXT:     let s: f32 = (n as f32) / 4.0f32;
// CHECK-NEXT:     let l: i64 = 3.99f32 as i64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g %ld\n".as_ptr(), r, s as f64, l) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     n = 2.75f64 as i32;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = ((n as f64) + 1.5f64) as i32;
// CHECK-NEXT:     n = [[V_0]];
// CHECK-NEXT:     println!("{}", n);
// CHECK-NEXT:     let mut u: u32 = 7;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = ((u as f64) * 0.5f64) as u32;
// CHECK-NEXT:     u = [[V_1]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), u) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
