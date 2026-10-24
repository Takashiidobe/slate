#include <stdarg.h>
#include <stdio.h>

typedef _BitInt(9) s9;
typedef unsigned _BitInt(17) u17;
typedef _BitInt(64) s64;
typedef _BitInt(100) s100;
typedef unsigned _BitInt(128) u128b;
typedef _BitInt(200) s200;
typedef unsigned _BitInt(300) u300;

struct pair { s9 a; s100 b; };

__attribute__((noinline)) s9 id9(s9 x) { return x; }
__attribute__((noinline)) u17 add17(u17 a, u17 b) { return a + b; }
__attribute__((noinline)) s100 mix100(s9 a, s100 b, s64 c) { return a + b * c; }
__attribute__((noinline)) u128b rot(u128b x, int n) { return (x << n) | (x >> (128 - n)); }
__attribute__((noinline)) s200 sum200(s200 a, s200 b, s200 c) { return a + b + c; }
__attribute__((noinline)) u300 big(u300 a, int k, u300 b) { return a * k - b; }
__attribute__((noinline)) struct pair mkpair(s9 a, s100 b) { struct pair p = {a, b}; return p; }
__attribute__((noinline)) s100 usepair(struct pair p) { return p.a + p.b; }

static long long lo(s200 v) { return (long long)v; }
static long long hi(s200 v) { return (long long)(v >> 100); }

static long long vsum(int n, ...) {
  va_list ap;
  long long total = 0;
  va_start(ap, n);
  for (int i = 0; i < n; i++) {
    switch (i % 5) {
    case 0: total += va_arg(ap, s9); break;
    case 1: total += va_arg(ap, u17); break;
    case 2: total += (long long)va_arg(ap, s64); break;
    case 3: total += (long long)va_arg(ap, u17); break;
    default: total += (long long)va_arg(ap, s64); break;
    }
  }
  va_end(ap);
  return total;
}

int main(void) {
  printf("%d\n", (int)id9(-200));
  printf("%u\n", (unsigned)add17(100000, 40000));
  printf("%lld\n", (long long)mix100(-5, 1000000000000LL, -3));
  u128b r = rot(((u128b)1 << 100) | 5, 7);
  printf("%llu %llu\n", (unsigned long long)r, (unsigned long long)(r >> 64));
  s200 s = sum200(((s200)1 << 150), -((s200)1 << 120), 77);
  printf("%lld %lld\n", lo(s), hi(s));
  u300 b = big(((u300)1 << 280) + 3, 5, 11);
  printf("%llu %llu\n", (unsigned long long)b, (unsigned long long)(b >> 256));
  struct pair p = mkpair(-7, ((s100)1 << 80) - 1);
  printf("%d %lld\n", (int)p.a, (long long)(p.b >> 70));
  printf("%lld\n", (long long)usepair(p));
  s100 (*fp)(s9, s100, s64) = mix100;
  printf("%lld\n", (long long)fp(3, 4, 5));
  printf("%lld\n", vsum(5, (s9)-3, (u17)70000, (s64)-5000000000LL, (u17)5, (s64)99));
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     b: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT: }
// CHECK-NEXT: fn id9(x: i16) -> i16 {
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn add17(a: u32, b: u32) -> u32 {
// CHECK-NEXT:     (a.wrapping_add(b) as u32) & 131071
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.mix100")]
// CHECK-NEXT: extern "C-unwind" fn mix100(
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     mut b: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     c: i64,
// CHECK-NEXT: ) -> bitint::BInt<100, 2, 16, bitint::A8> {
// CHECK-NEXT:     return <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(a as i128)
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             b.wrapping_mul(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(c as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT: }
// CHECK-NEXT: fn rot(
// CHECK-NEXT:     mut x: bitint::BUint<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     n: i32,
// CHECK-NEXT: ) -> bitint::BUint<128, 2, 16, bitint::A8> {
// CHECK-NEXT:     return x.shl_bits(n as usize).bitor_bits(x.shr_bits((128 - n) as usize));
// CHECK-NEXT: }
// CHECK-NEXT: fn sum200(
// CHECK-NEXT:     mut a: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     c: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BInt<200, 4, 32, bitint::A8> {
// CHECK-NEXT:     return a.wrapping_add(b).wrapping_add(c);
// CHECK-NEXT: }
// CHECK-NEXT: fn big(
// CHECK-NEXT:     mut a: bitint::BUint<300, 5, 40, bitint::A8>,
// CHECK-NEXT:     k: i32,
// CHECK-NEXT:     b: bitint::BUint<300, 5, 40, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BUint<300, 5, 40, bitint::A8> {
// CHECK-NEXT:     return a
// CHECK-NEXT:         .wrapping_mul(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 300,
// CHECK-NEXT:                 5,
// CHECK-NEXT:                 40,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<300, 5, 40, bitint::A8>>::from_i128(k as i128)),
// CHECK-NEXT:         )
// CHECK-NEXT:         .wrapping_sub(b);
// CHECK-NEXT: }
// CHECK-NEXT: fn mkpair(a: i16, b: bitint::BInt<100, 2, 16, bitint::A8>) -> pair {
// CHECK-NEXT:     let p: pair = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].a = a;
// CHECK-NEXT:         [[T_0]].b = b;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     return p;
// CHECK-NEXT: }
// CHECK-NEXT: fn usepair(p: pair) -> bitint::BInt<100, 2, 16, bitint::A8> {
// CHECK-NEXT:     return <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(p.a as i128)
// CHECK-NEXT:         .wrapping_add(p.b);
// CHECK-NEXT: }
// CHECK-NEXT: fn lo(mut v: bitint::BInt<200, 4, 32, bitint::A8>) -> i64 {
// CHECK-NEXT:     v.to_i128() as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn hi(mut v: bitint::BInt<200, 4, 32, bitint::A8>) -> i64 {
// CHECK-NEXT:     v.shr_bits(100 as usize).to_i128() as i64
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn vsum(n: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         match i % 5 {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 let [[V_0:__v[0-9]+]]: i16 = unsafe { (ap.next_arg::<i32>() as i16) << 7 >> 7 };
// CHECK-NEXT:                 total += [[V_0]] as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 let [[V_1:__v[0-9]+]]: u32 = unsafe { ap.next_arg::<u32>() };
// CHECK-NEXT:                 total += ([[V_1]] as u64) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 total += (unsafe { ap.next_arg::<i64>() }) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 total += ((unsafe { ap.next_arg::<u32>() }) as u64) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 total += (unsafe { ap.next_arg::<i64>() }) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", id9(- 200i16) as i32);
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), add17(100000u32, 40000u32) as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             mix100(
// CHECK-NEXT:                     -5i16,
// CHECK-NEXT:                     <bitint::BInt<
// CHECK-NEXT:                         100,
// CHECK-NEXT:                         2,
// CHECK-NEXT:                         16,
// CHECK-NEXT:                         bitint::A8,
// CHECK-NEXT:                     >>::from_i128(1000000000000 as i128),
// CHECK-NEXT:                     -3i64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_i128() as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut r: bitint::BUint<128, 2, 16, bitint::A8> = rot(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(1 as i128))
// CHECK-NEXT:             .shl_bits(100 as usize)
// CHECK-NEXT:             .bitor_bits(
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     128,
// CHECK-NEXT:                     2,
// CHECK-NEXT:                     16,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                     <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(5 as i128),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:         7i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%llu %llu\n".as_ptr(),
// CHECK-NEXT:             r.to_u128() as u64,
// CHECK-NEXT:             r.shr_bits(64 as usize).to_u128() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let s: bitint::BInt<200, 4, 32, bitint::A8> = sum200(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:             .shl_bits(150 as usize),
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:             .shl_bits(120 as usize)
// CHECK-NEXT:             .wrapping_neg(),
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(77 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), lo(s), hi(s)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut b: bitint::BUint<300, 5, 40, bitint::A8> = big(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             300,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<300, 5, 40, bitint::A8>>::from_i128(1 as i128))
// CHECK-NEXT:             .shl_bits(280 as usize)
// CHECK-NEXT:             .wrapping_add(
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     300,
// CHECK-NEXT:                     5,
// CHECK-NEXT:                     40,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                     <bitint::BInt<300, 5, 40, bitint::A8>>::from_i128(3 as i128),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:         5i32,
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             300,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<300, 5, 40, bitint::A8>>::from_i128(11 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%llu %llu\n".as_ptr(),
// CHECK-NEXT:             b.to_u128() as u64,
// CHECK-NEXT:             b.shr_bits(256 as usize).to_u128() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut p: pair = mkpair(
// CHECK-NEXT:         -7i16,
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:             .shl_bits(80 as usize)
// CHECK-NEXT:             .wrapping_sub(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(1 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %lld\n".as_ptr(),
// CHECK-NEXT:             p.a as i32,
// CHECK-NEXT:             p.b.shr_bits(70 as usize).to_i128() as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), usepair(p).to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let fp: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// CHECK-NEXT:             i16,
// CHECK-NEXT:             bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:             i64,
// CHECK-NEXT:         ) -> bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     > = Some(mix100);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             fp
// CHECK-NEXT:                 .unwrap()(
// CHECK-NEXT:                     3i16,
// CHECK-NEXT:                     <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(4 as i128),
// CHECK-NEXT:                     5i64,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_i128() as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             vsum(5i32, -3i32, 70000u32, -5000000000i64, 5u32, 99i64),
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
