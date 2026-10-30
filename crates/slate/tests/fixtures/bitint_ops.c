#include <stdio.h>

#define OPS(NAME, T, FMT, CAST)                                               \
  static void NAME(T a, T b) {                                                \
    T r;                                                                      \
    r = a + b;  printf(FMT " ", (CAST)r);                                     \
    r = a - b;  printf(FMT " ", (CAST)r);                                     \
    r = a * b;  printf(FMT " ", (CAST)r);                                     \
    r = b ? a / b : 0; printf(FMT " ", (CAST)r);                              \
    r = b ? a % b : 0; printf(FMT " ", (CAST)r);                              \
    r = a & b;  printf(FMT " ", (CAST)r);                                     \
    r = a | b;  printf(FMT " ", (CAST)r);                                     \
    r = a ^ b;  printf(FMT " ", (CAST)r);                                     \
    r = ~a;     printf(FMT " ", (CAST)r);                                     \
    r = -a;     printf(FMT " ", (CAST)r);                                     \
    r = a; r += b; r -= a; r *= 3; r ^= b; r |= a; r &= b;                   \
    printf(FMT " ", (CAST)r);                                                 \
    printf("%d%d%d%d%d%d\n", a == b, a != b, a < b, a <= b, a > b, a >= b);   \
  }

OPS(s5, _BitInt(5), "%lld", long long)
OPS(u5, unsigned _BitInt(5), "%llu", unsigned long long)
OPS(s9, _BitInt(9), "%lld", long long)
OPS(u9, unsigned _BitInt(9), "%llu", unsigned long long)
OPS(s33, _BitInt(33), "%lld", long long)
OPS(u33, unsigned _BitInt(33), "%llu", unsigned long long)
OPS(s64, _BitInt(64), "%lld", long long)
OPS(u64, unsigned _BitInt(64), "%llu", unsigned long long)
OPS(s100, _BitInt(100), "%lld", long long)
OPS(u100, unsigned _BitInt(100), "%llu", unsigned long long)
OPS(s128, _BitInt(128), "%lld", long long)
OPS(u128, unsigned _BitInt(128), "%llu", unsigned long long)
OPS(s200, _BitInt(200), "%lld", long long)
OPS(u200, unsigned _BitInt(200), "%llu", unsigned long long)

static const _BitInt(200) g_add = (_BitInt(200))5 + 7;
static const unsigned _BitInt(100) g_cmp = (unsigned _BitInt(100))3 < 4;

int main(void) {
  s5(7, -3); s5(-16, 15); u5(31, 3); u5(2, 31);
  s9(255, -256); s9(-100, 7); u9(511, 2); u9(3, 500);
  s33(4294967295LL, -5); u33(8589934591ULL, 3);
  s64(9223372036854775807LL, 2); u64(18446744073709551615ULL, 5);
  s100(123456789, -987); u100(123456789, 987);
  s128(-123456789, 1000); u128(123456789, 1000);
  s200(123456789, -1000); u200(123456789, 1000);
  printf("%lld %llu\n", (long long)g_add, (unsigned long long)g_cmp);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn s5(a: i8, b: i8) {
// CHECK-NEXT:     let mut r: i8 = (a.wrapping_add(b) as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_sub(b) as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_mul(b) as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((if b != 0 { ((a.wrapping_div(b) as i8) << 3 >> 3) as i32 } else { 0i32 })
// CHECK-NEXT:         as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((if b != 0 { ((a.wrapping_rem(b) as i8) << 3 >> 3) as i32 } else { 0i32 })
// CHECK-NEXT:         as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (!a as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_neg() as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_add(b) as i8) << 3 >> 3;
// CHECK-NEXT:     r = (r.wrapping_sub(a) as i8) << 3 >> 3;
// CHECK-NEXT:     r = (((r as i32) * 3) as i8) << 3 >> 3;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u5(a: u8, b: u8) {
// CHECK-NEXT:     let mut r: u8 = (a.wrapping_add(b) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_sub(b) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_mul(b) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((((if b != 0 { (((a.wrapping_div(b) as u8) & 31) as u32) as i32 } else { 0i32 })
// CHECK-NEXT:         as i8) << 3 >> 3) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((((if b != 0 { (((a.wrapping_rem(b) as u8) & 31) as u32) as i32 } else { 0i32 })
// CHECK-NEXT:         as i8) << 3 >> 3) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (!a as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_neg() as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_add(b) as u8) & 31;
// CHECK-NEXT:     r = (r.wrapping_sub(a) as u8) & 31;
// CHECK-NEXT:     r = ((((((r as u32) as i32) * 3) as i8) << 3 >> 3) as u8) & 31;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn s9(a: i16, b: i16) {
// CHECK-NEXT:     let mut r: i16 = (a.wrapping_add(b) as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_sub(b) as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_mul(b) as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((if b != 0 { ((a.wrapping_div(b) as i16) << 7 >> 7) as i32 } else { 0i32 })
// CHECK-NEXT:         as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((if b != 0 { ((a.wrapping_rem(b) as i16) << 7 >> 7) as i32 } else { 0i32 })
// CHECK-NEXT:         as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (!a as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_neg() as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_add(b) as i16) << 7 >> 7;
// CHECK-NEXT:     r = (r.wrapping_sub(a) as i16) << 7 >> 7;
// CHECK-NEXT:     r = (((r as i32) * 3) as i16) << 7 >> 7;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u9(a: u16, b: u16) {
// CHECK-NEXT:     let mut r: u16 = (a.wrapping_add(b) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_sub(b) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_mul(b) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((((if b != 0 {
// CHECK-NEXT:         (((a.wrapping_div(b) as u16) & 511) as u32) as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0i32
// CHECK-NEXT:     }) as i16) << 7 >> 7) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = ((((if b != 0 {
// CHECK-NEXT:         (((a.wrapping_rem(b) as u16) & 511) as u32) as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0i32
// CHECK-NEXT:     }) as i16) << 7 >> 7) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (!a as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_neg() as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_add(b) as u16) & 511;
// CHECK-NEXT:     r = (r.wrapping_sub(a) as u16) & 511;
// CHECK-NEXT:     r = ((((((r as u32) as i32) * 3) as i16) << 7 >> 7) as u16) & 511;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn s33(a: i64, b: i64) {
// CHECK-NEXT:     let mut r: i64 = (a.wrapping_add(b) as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_sub(b) as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_mul(b) as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { (a.wrapping_div(b) as i64) << 31 >> 31 } else { 0i64 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { (a.wrapping_rem(b) as i64) << 31 >> 31 } else { 0i64 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (!a as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_neg() as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_add(b) as i64) << 31 >> 31;
// CHECK-NEXT:     r = (r.wrapping_sub(a) as i64) << 31 >> 31;
// CHECK-NEXT:     r = (r.wrapping_mul(3i64) as i64) << 31 >> 31;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u33(a: u64, b: u64) {
// CHECK-NEXT:     let mut r: u64 = (a.wrapping_add(b) as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_sub(b) as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_mul(b) as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { (a.wrapping_div(b) as u64) & 8589934591 } else { 0u64 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { (a.wrapping_rem(b) as u64) & 8589934591 } else { 0u64 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (!a as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_neg() as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_add(b) as u64) & 8589934591;
// CHECK-NEXT:     r = (r.wrapping_sub(a) as u64) & 8589934591;
// CHECK-NEXT:     r = (r.wrapping_mul(3u64) as u64) & 8589934591;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn s64(a: i64, b: i64) {
// CHECK-NEXT:     let mut r: i64 = a + b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a - b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a * b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { a / b } else { 0 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { a % b } else { 0 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = !a;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = -a;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r += b;
// CHECK-NEXT:     r -= a;
// CHECK-NEXT:     r *= 3;
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u64(a: u64, b: u64) {
// CHECK-NEXT:     let mut r: u64 = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { a / b } else { 0 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b != 0 { a % b } else { 0 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a & b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a | b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a ^ b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = !a;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_add(b);
// CHECK-NEXT:     r = r.wrapping_sub(a);
// CHECK-NEXT:     r = r.wrapping_mul(3);
// CHECK-NEXT:     r ^= b;
// CHECK-NEXT:     r |= a;
// CHECK-NEXT:     r &= b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a == b) as i32, (a != b) as i32, (a < b) as i32, (a <= b) as
// CHECK-NEXT:         i32, (a > b) as i32, (a >= b) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn s100(
// CHECK-NEXT:     mut a: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     mut b: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<100, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BInt<100, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_div(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(0 as i128)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BInt<100, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_rem(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(0 as i128)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitxor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitnot();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].wrapping_add(b);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].wrapping_sub(a);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_2]].wrapping_mul(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(3 as i128));
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_3]].bitxor_bits(b);
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_4]].bitor_bits(a);
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_5]].bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a.compare(b) == 0) as i32, (a.compare(b) != 0) as i32, (a
// CHECK-NEXT:         .compare(b) < 0) as i32, (a.compare(b) <= 0) as i32, (a.compare(b) > 0) as i32,
// CHECK-NEXT:         (a.compare(b) >= 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u100(
// CHECK-NEXT:     mut a: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     mut b: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BUint<100, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_div(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_rem(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitxor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitnot();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].wrapping_add(b);
// CHECK-NEXT:     let mut [[V_1]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].wrapping_sub(a);
// CHECK-NEXT:     let mut [[V_2]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_2]]
// CHECK-NEXT:         .wrapping_mul(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 100,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(3 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut [[V_3]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_3]].bitxor_bits(b);
// CHECK-NEXT:     let mut [[V_4]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_4]].bitor_bits(a);
// CHECK-NEXT:     let mut [[V_5]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_5]].bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a.compare(b) == 0) as i32, (a.compare(b) != 0) as i32, (a
// CHECK-NEXT:         .compare(b) < 0) as i32, (a.compare(b) <= 0) as i32, (a.compare(b) > 0) as i32,
// CHECK-NEXT:         (a.compare(b) >= 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn s128(
// CHECK-NEXT:     mut a: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     mut b: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<128, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_div(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(0 as i128)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_rem(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(0 as i128)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitxor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitnot();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].wrapping_add(b);
// CHECK-NEXT:     let mut [[V_1]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].wrapping_sub(a);
// CHECK-NEXT:     let mut [[V_2]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_2]].wrapping_mul(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(3 as i128));
// CHECK-NEXT:     let mut [[V_3]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_3]].bitxor_bits(b);
// CHECK-NEXT:     let mut [[V_4]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_4]].bitor_bits(a);
// CHECK-NEXT:     let mut [[V_5]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_5]].bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a.compare(b) == 0) as i32, (a.compare(b) != 0) as i32, (a
// CHECK-NEXT:         .compare(b) < 0) as i32, (a.compare(b) <= 0) as i32, (a.compare(b) > 0) as i32,
// CHECK-NEXT:         (a.compare(b) >= 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u128(
// CHECK-NEXT:     mut a: bitint::BUint<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     mut b: bitint::BUint<128, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BUint<128, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_div(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_rem(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitxor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitnot();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].wrapping_add(b);
// CHECK-NEXT:     let mut [[V_1]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].wrapping_sub(a);
// CHECK-NEXT:     let mut [[V_2]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_2]]
// CHECK-NEXT:         .wrapping_mul(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 128,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(3 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut [[V_3]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_3]].bitxor_bits(b);
// CHECK-NEXT:     let mut [[V_4]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_4]].bitor_bits(a);
// CHECK-NEXT:     let mut [[V_5]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_5]].bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a.compare(b) == 0) as i32, (a.compare(b) != 0) as i32, (a
// CHECK-NEXT:         .compare(b) < 0) as i32, (a.compare(b) <= 0) as i32, (a.compare(b) > 0) as i32,
// CHECK-NEXT:         (a.compare(b) >= 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn s200(
// CHECK-NEXT:     mut a: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     mut b: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<200, 4, 32, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_div(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(0 as i128)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_rem(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(0 as i128)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitxor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitnot();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].wrapping_add(b);
// CHECK-NEXT:     let mut [[V_1]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].wrapping_sub(a);
// CHECK-NEXT:     let mut [[V_2]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_2]].wrapping_mul(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(3 as i128));
// CHECK-NEXT:     let mut [[V_3]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_3]].bitxor_bits(b);
// CHECK-NEXT:     let mut [[V_4]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_4]].bitor_bits(a);
// CHECK-NEXT:     let mut [[V_5]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_5]].bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a.compare(b) == 0) as i32, (a.compare(b) != 0) as i32, (a
// CHECK-NEXT:         .compare(b) < 0) as i32, (a.compare(b) <= 0) as i32, (a.compare(b) > 0) as i32,
// CHECK-NEXT:         (a.compare(b) >= 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn u200(
// CHECK-NEXT:     mut a: bitint::BUint<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     mut b: bitint::BUint<200, 4, 32, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BUint<200, 4, 32, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.wrapping_add(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_mul(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BUint<200, 4, 32, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_div(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = if b.compare(<bitint::BUint<200, 4, 32, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         a.wrapping_rem(b)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitxor_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.bitnot();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].wrapping_add(b);
// CHECK-NEXT:     let mut [[V_1]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].wrapping_sub(a);
// CHECK-NEXT:     let mut [[V_2]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_2]]
// CHECK-NEXT:         .wrapping_mul(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 200,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(3 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut [[V_3]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_3]].bitxor_bits(b);
// CHECK-NEXT:     let mut [[V_4]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_4]].bitor_bits(a);
// CHECK-NEXT:     let mut [[V_5]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_5]].bitand_bits(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}{}{}{}{}{}", (a.compare(b) == 0) as i32, (a.compare(b) != 0) as i32, (a
// CHECK-NEXT:         .compare(b) < 0) as i32, (a.compare(b) <= 0) as i32, (a.compare(b) > 0) as i32,
// CHECK-NEXT:         (a.compare(b) >= 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_add: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:     200,
// CHECK-NEXT:     4,
// CHECK-NEXT:     32,
// CHECK-NEXT:     bitint::A8,
// CHECK-NEXT: >>::from_i128(5i32 as i128)
// CHECK-NEXT:     .wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(7i32 as i128));
// CHECK-NEXT: static mut g_cmp: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:     100,
// CHECK-NEXT:     2,
// CHECK-NEXT:     16,
// CHECK-NEXT:     bitint::A8,
// CHECK-NEXT: >>::from_u128(
// CHECK-NEXT:     (<bitint::BUint<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(3i32 as i128))
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 100,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(
// CHECK-NEXT:                 <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(4i32 as i128),
// CHECK-NEXT:             ),
// CHECK-NEXT:         ) < 0) as u128,
// CHECK-NEXT: );
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     s5(7i8, -3i8);
// CHECK-NEXT:     s5(-16i8, 15i8);
// CHECK-NEXT:     u5(31u8, 3u8);
// CHECK-NEXT:     u5(2u8, 31u8);
// CHECK-NEXT:     s9(255i16, -256i16);
// CHECK-NEXT:     s9(-100i16, 7i16);
// CHECK-NEXT:     u9(511u16, 2u16);
// CHECK-NEXT:     u9(3u16, 500u16);
// CHECK-NEXT:     s33(4294967295i64, -5i64);
// CHECK-NEXT:     u33(8589934591u64, 3u64);
// CHECK-NEXT:     s64(9223372036854775807i64, 2i64);
// CHECK-NEXT:     u64(18446744073709551615u64, 5u64);
// CHECK-NEXT:     s100(
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(123456789 as i128),
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(-987 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     u100(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(
// CHECK-NEXT:             <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(123456789 as i128),
// CHECK-NEXT:         ),
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(987 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     s128(
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-123456789 as i128),
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(1000 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     u128(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(
// CHECK-NEXT:             <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(123456789 as i128),
// CHECK-NEXT:         ),
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(1000 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     s200(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(123456789 as i128),
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(-1000 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     u200(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(
// CHECK-NEXT:             <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(123456789 as i128),
// CHECK-NEXT:         ),
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1000 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld %llu\n".as_ptr(),
// CHECK-NEXT:             unsafe { g_add }.to_i128() as i64,
// CHECK-NEXT:             unsafe { g_cmp }.to_u128() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
