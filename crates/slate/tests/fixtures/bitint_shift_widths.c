#include <stdio.h>

#define SHIFTS(NAME, T, FMT, CAST)                                            \
  static void NAME(T a, int n, unsigned _BitInt(7) m) {                       \
    T r;                                                                      \
    r = a << n; printf(FMT " ", (CAST)r);                                     \
    r = a >> n; printf(FMT " ", (CAST)r);                                     \
    r = a << m; printf(FMT " ", (CAST)r);                                     \
    r = a >> m; printf(FMT " ", (CAST)r);                                     \
    r = a; r <<= 1; printf(FMT " ", (CAST)r);                                 \
    r = a; r >>= 1; printf(FMT "\n", (CAST)r);                                \
  }

SHIFTS(s5, _BitInt(5), "%lld", long long)
SHIFTS(u5, unsigned _BitInt(5), "%llu", unsigned long long)
SHIFTS(s9, _BitInt(9), "%lld", long long)
SHIFTS(u9, unsigned _BitInt(9), "%llu", unsigned long long)
SHIFTS(s33, _BitInt(33), "%lld", long long)
SHIFTS(u33, unsigned _BitInt(33), "%llu", unsigned long long)
SHIFTS(s64, _BitInt(64), "%lld", long long)
SHIFTS(u64, unsigned _BitInt(64), "%llu", unsigned long long)
SHIFTS(s100, _BitInt(100), "%lld", long long)
SHIFTS(u100, unsigned _BitInt(100), "%llu", unsigned long long)
SHIFTS(s128, _BitInt(128), "%lld", long long)
SHIFTS(u128, unsigned _BitInt(128), "%llu", unsigned long long)
SHIFTS(s200, _BitInt(200), "%lld", long long)
SHIFTS(u200, unsigned _BitInt(200), "%llu", unsigned long long)

static const _BitInt(200) g_shl = (_BitInt(200))3 << 70;
static const unsigned _BitInt(100) g_shr = (unsigned _BitInt(100))1024 >> 4;
static const _BitInt(9) g_small = (_BitInt(9))3 << 7;

int main(void) {
  s5(5, 2, 1); s5(-7, 1, 3); u5(21, 3, 2); u5(31, 1, 4);
  s9(100, 3, 2); s9(-200, 2, 5); u9(300, 2, 1); u9(511, 4, 3);
  s33(4294967295LL, 3, 2); u33(8589934591ULL, 4, 1);
  s64(-9223372036854775807LL, 1, 2); u64(18446744073709551615ULL, 5, 3);
  s100(-123456789, 40, 20); u100(123456789, 40, 20);
  s128(-123456789, 70, 3); u128(123456789, 70, 3);
  s200(-123456789, 130, 100); u200(123456789, 130, 100);
  printf("%lld %llu %lld\n", (long long)(g_shl >> 70), (unsigned long long)g_shr,
         (long long)g_small);
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
// CHECK-NEXT: fn s5(a: i8, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: i8 = (a.wrapping_shl((n as usize) as u32) as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((n as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_shl((m as usize) as u32) as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((m as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_shl(1u32) as i8) << 3 >> 3;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_shr(1u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u5(a: u8, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: u8 = (a.wrapping_shl((n as usize) as u32) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((n as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_shl((m as usize) as u32) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((m as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_shl(1u32) as u8) & 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_shr(1u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s9(a: i16, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: i16 = (a.wrapping_shl((n as usize) as u32) as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((n as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_shl((m as usize) as u32) as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((m as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_shl(1u32) as i16) << 7 >> 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_shr(1u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u9(a: u16, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: u16 = (a.wrapping_shl((n as usize) as u32) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((n as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_shl((m as usize) as u32) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((m as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_shl(1u32) as u16) & 511;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_shr(1u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s33(a: i64, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: i64 = (a.wrapping_shl((n as usize) as u32) as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((n as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_shl((m as usize) as u32) as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((m as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_shl(1u32) as i64) << 31 >> 31;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_shr(1u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u33(a: u64, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: u64 = (a.wrapping_shl((n as usize) as u32) as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((n as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = (a.wrapping_shl((m as usize) as u32) as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.wrapping_shr((m as usize) as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = (r.wrapping_shl(1u32) as u64) & 8589934591;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r = r.wrapping_shr(1u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s64(a: i64, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: i64 = a << n;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a >> n;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a << m;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a >> m;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r <<= 1i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r >>= 1i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u64(a: u64, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: u64 = a << n;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a >> n;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a << m;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a >> m;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r <<= 1i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     r >>= 1i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s100(mut a: bitint::BInt<100, 2, 16, bitint::A8>, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: bitint::BInt<100, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.shl_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shl_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].shl_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].shr_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u100(mut a: bitint::BUint<100, 2, 16, bitint::A8>, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: bitint::BUint<100, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.shl_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shl_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].shl_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_1]]: bitint::BUint<100, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].shr_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s128(mut a: bitint::BInt<128, 2, 16, bitint::A8>, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: bitint::BInt<128, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.shl_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shl_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].shl_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_1]]: bitint::BInt<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].shr_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u128(mut a: bitint::BUint<128, 2, 16, bitint::A8>, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: bitint::BUint<128, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.shl_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shl_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].shl_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_1]]: bitint::BUint<128, 2, 16, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].shr_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s200(mut a: bitint::BInt<200, 4, 32, bitint::A8>, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: bitint::BInt<200, 4, 32, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.shl_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shl_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].shl_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld ".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_1]]: bitint::BInt<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].shr_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), r.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u200(mut a: bitint::BUint<200, 4, 32, bitint::A8>, n: i32, m: u8) {
// CHECK-NEXT:     let mut r: bitint::BUint<200, 4, 32, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = a.shl_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(n as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shl_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a.shr_bits(m as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_0]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_0]].shl_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu ".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r = a;
// CHECK-NEXT:     let mut [[V_1]]: bitint::BUint<200, 4, 32, bitint::A8> = r;
// CHECK-NEXT:     r = [[V_1]].shr_bits(1 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), r.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_shl: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:     200,
// CHECK-NEXT:     4,
// CHECK-NEXT:     32,
// CHECK-NEXT:     bitint::A8,
// CHECK-NEXT: >>::from_i128(3i32 as i128)
// CHECK-NEXT:     .shl_bits(70i32 as usize);
// CHECK-NEXT: static mut g_shr: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:     100,
// CHECK-NEXT:     2,
// CHECK-NEXT:     16,
// CHECK-NEXT:     bitint::A8,
// CHECK-NEXT: >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(1024i32 as i128))
// CHECK-NEXT:     .shr_bits(4i32 as usize);
// CHECK-NEXT: static mut g_small: i16 = (3i16.wrapping_shl((7i32 as usize) as u32) as i16) << 7 >> 7;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     s5(5i8, 2i32, 1u8);
// CHECK-NEXT:     s5(-7i8, 1i32, 3u8);
// CHECK-NEXT:     u5(21u8, 3i32, 2u8);
// CHECK-NEXT:     u5(31u8, 1i32, 4u8);
// CHECK-NEXT:     s9(100i16, 3i32, 2u8);
// CHECK-NEXT:     s9(-200i16, 2i32, 5u8);
// CHECK-NEXT:     u9(300u16, 2i32, 1u8);
// CHECK-NEXT:     u9(511u16, 4i32, 3u8);
// CHECK-NEXT:     s33(4294967295i64, 3i32, 2u8);
// CHECK-NEXT:     u33(8589934591u64, 4i32, 1u8);
// CHECK-NEXT:     s64(-9223372036854775807i64, 1i32, 2u8);
// CHECK-NEXT:     u64(18446744073709551615u64, 5i32, 3u8);
// CHECK-NEXT:     s100(
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(-123456789 as i128),
// CHECK-NEXT:         40i32,
// CHECK-NEXT:         20u8,
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
// CHECK-NEXT:         40i32,
// CHECK-NEXT:         20u8,
// CHECK-NEXT:     );
// CHECK-NEXT:     s128(
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-123456789 as i128),
// CHECK-NEXT:         70i32,
// CHECK-NEXT:         3u8,
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
// CHECK-NEXT:         70i32,
// CHECK-NEXT:         3u8,
// CHECK-NEXT:     );
// CHECK-NEXT:     s200(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(-123456789 as i128),
// CHECK-NEXT:         130i32,
// CHECK-NEXT:         100u8,
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
// CHECK-NEXT:         130i32,
// CHECK-NEXT:         100u8,
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld %llu %lld\n".as_ptr(),
// CHECK-NEXT:             unsafe { g_shl }.shr_bits(70 as usize).to_i128() as i64,
// CHECK-NEXT:             unsafe { g_shr }.to_u128() as u64,
// CHECK-NEXT:             g_small as i64,
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
