#include <stdio.h>
#include <stdbool.h>

static _Bool to_bool(int x) { return x; }
static _Bool to_bool_u8(unsigned char x) { return x; }
static _Bool to_bool_ll(long long x) { return (_Bool)x; }
static int from_bool(_Bool b) { return b + 1; }
static signed char trunc_sc(int x) { return (signed char)x; }
static unsigned char trunc_uc(int x) { return x; }
static short trunc_s(long long x) { return (short)x; }
static unsigned widen_u(signed char c) { return c; }
static long long widen_ll(unsigned short s) { return s; }
static unsigned long long widen_ull(int x) { return x; }
static int reinterp(unsigned x) { return (int)x; }
static unsigned reinterp_u(int x) { return x; }
static int promote(unsigned char a, unsigned char b) { return a - b; }
static int promote_s(short a, short b) { return a * b; }

int main(void) {
  printf("%d %d %d\n", to_bool(256), to_bool(0), to_bool(-7));
  printf("%d %d\n", to_bool_u8(0), to_bool_u8(200));
  printf("%d %d\n", to_bool_ll(1LL << 40), to_bool_ll(0));
  _Bool b = 42;
  printf("%d %d\n", b, from_bool(b));
  b = to_bool(0);
  int bi = b;
  printf("%d\n", bi);
  printf("%d %d %d\n", trunc_sc(200), trunc_uc(-1), trunc_s(0x12345678LL));
  printf("%u %lld %llu\n", widen_u(-3), widen_ll(65535), widen_ull(-1));
  printf("%d %u\n", reinterp(0xFFFFFFFFu), reinterp_u(-2));
  printf("%d %d\n", promote(1, 2), promote_s(300, 300));
  unsigned char uc = 255;
  uc = uc + 1;
  printf("%d\n", uc);
  _Bool c = !b;
  printf("%d %d\n", c, !bi);
  return to_bool(3) + to_bool(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn to_bool(x: i32) -> bool {
// CHECK-NEXT:     x != 0
// CHECK-NEXT: }
// CHECK-NEXT: fn to_bool_u8(x: u8) -> bool {
// CHECK-NEXT:     x != 0
// CHECK-NEXT: }
// CHECK-NEXT: fn to_bool_ll(x: i64) -> bool {
// CHECK-NEXT:     x != 0
// CHECK-NEXT: }
// CHECK-NEXT: fn from_bool(b: bool) -> i32 {
// CHECK-NEXT:     (b as i32) + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn trunc_sc(x: i32) -> i8 {
// CHECK-NEXT:     x as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn trunc_uc(x: i32) -> u8 {
// CHECK-NEXT:     (x as i8) as u8
// CHECK-NEXT: }
// CHECK-NEXT: fn trunc_s(x: i64) -> i16 {
// CHECK-NEXT:     x as i16
// CHECK-NEXT: }
// CHECK-NEXT: fn widen_u(c: i8) -> u32 {
// CHECK-NEXT:     (c as i32) as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn widen_ll(s: u16) -> i64 {
// CHECK-NEXT:     (s as u64) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn widen_ull(x: i32) -> u64 {
// CHECK-NEXT:     (x as i64) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn reinterp(x: u32) -> i32 {
// CHECK-NEXT:     x as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn reinterp_u(x: i32) -> u32 {
// CHECK-NEXT:     x as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn promote(a: u8, b: u8) -> i32 {
// CHECK-NEXT:     ((a as u32) as i32) - ((b as u32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn promote_s(a: i16, b: i16) -> i32 {
// CHECK-NEXT:     (a as i32) * (b as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", to_bool(256 as i32) as i32, to_bool(0 as i32) as i32, to_bool(- (7 as
// CHECK-NEXT:         i32)) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", to_bool_u8(0 as u8) as i32, to_bool_u8(((200 as i32) as i8) as u8) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", to_bool_ll((1 as i64) << (40 as i32)) as i32, to_bool_ll(0 as i64) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut b: bool = (42 as i32) != (0 as i32);
// CHECK-NEXT:     println!("{} {}", b as i32, from_bool(b));
// CHECK-NEXT:     b = to_bool(0 as i32);
// CHECK-NEXT:     let bi: i32 = b as i32;
// CHECK-NEXT:     println!("{}", bi);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", trunc_sc(200 as i32) as i32, (trunc_uc(- (1 as i32)) as u32) as i32,
// CHECK-NEXT:         trunc_s(305419896 as i64) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %lld %llu\n".as_ptr(),
// CHECK-NEXT:             widen_u(-3 as i8),
// CHECK-NEXT:             widen_ll(((65535 as i32) as i16) as u16),
// CHECK-NEXT:             widen_ull(-(1 as i32)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%d %u\n".as_ptr(), reinterp(4294967295 as u32), reinterp_u(-(2 as i32)))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", promote(1 as u8, 2 as u8), promote_s(300 as i16, 300 as i16));
// CHECK-NEXT:     let mut uc: u8 = ((255 as i32) as i8) as u8;
// CHECK-NEXT:     uc = ((((uc as u32) as i32) + 1) as i8) as u8;
// CHECK-NEXT:     println!("{}", (uc as u32) as i32);
// CHECK-NEXT:     let c: bool = !b;
// CHECK-NEXT:     println!("{} {}", c as i32, ! (bi != 0) as i32);
// CHECK-NEXT:     (to_bool(3 as i32) as i32) + (to_bool(0 as i32) as i32)
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
