#include <stdio.h>

static __int128 add128(__int128 a, __int128 b) { return a + b; }

static unsigned __int128 mul128(unsigned __int128 a, unsigned __int128 b) {
  return a * b;
}

static void print128(unsigned __int128 v) {
  unsigned long long hi = (unsigned long long)(v >> 64);
  unsigned long long lo = (unsigned long long)v;
  printf("%llu:%llu\n", hi, lo);
}

int main(void) {
  __int128 a   = (__int128)9000000000000000000LL;
  __int128 b   = (__int128)9000000000000000000LL;
  __int128 sum = add128(a, b);
  print128((unsigned __int128)sum);

  unsigned __int128 x    = (unsigned __int128)1000000000000ULL;
  unsigned __int128 y    = (unsigned __int128)1000000000000ULL;
  unsigned __int128 prod = mul128(x, y);
  print128(prod);

  int cmp = (sum > 0) ? 1 : 0;
  printf("%d\n", cmp);

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
// CHECK-NEXT: fn add128(a: i128, b: i128) -> i128 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: fn mul128(a: u128, b: u128) -> u128 {
// CHECK-NEXT:     a.wrapping_mul(b)
// CHECK-NEXT: }
// CHECK-NEXT: fn print128(v: u128) {
// CHECK-NEXT:     let hi: u64 = (v >> 64i32) as u64;
// CHECK-NEXT:     let lo: u64 = v as u64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu:%llu\n".as_ptr(), hi, lo) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i128 = 9000000000000000000;
// CHECK-NEXT:     let b: i128 = 9000000000000000000;
// CHECK-NEXT:     let sum: i128 = add128(a, b);
// CHECK-NEXT:     print128(sum as u128);
// CHECK-NEXT:     let x: u128 = 1000000000000;
// CHECK-NEXT:     let y: u128 = 1000000000000;
// CHECK-NEXT:     let prod: u128 = mul128(x, y);
// CHECK-NEXT:     print128(prod);
// CHECK-NEXT:     let cmp: i32 = if sum > 0 { 1 } else { 0 };
// CHECK-NEXT:     println!("{}", cmp);
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
