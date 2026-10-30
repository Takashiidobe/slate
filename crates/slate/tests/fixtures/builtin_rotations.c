#include <stdio.h>
#include <stdint.h>

#define TEST(W, T, X) do { \
  T x = (T)(X); \
  unsigned counts[] = {0, 1, W - 1, W, W + 1, 255}; \
  for (unsigned i = 0; i < 6; ++i) { \
    unsigned n = counts[i]; \
    printf("%u %u %llu %llu\n", W, n, \
      (unsigned long long)__builtin_rotateleft##W(x, n), \
      (unsigned long long)__builtin_rotateright##W(x, n)); \
  } \
} while (0)

int main(void) {
  TEST(8, uint8_t, 0x93);
  TEST(16, uint16_t, 0x9137);
  TEST(32, uint32_t, 0x9137abcd);
  TEST(64, uint64_t, 0x9137abcdef012345ULL);
  unsigned x = 3, n = 1;
  unsigned r = __builtin_rotateleft32(x++, n++);
  printf("%u %u %u\n", r, x, n);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_rotateleft8([[ARG_0]]: u8, [[ARG_1:arg[0-9]+]]: u8) -> u8;
// CHECK-NEXT:     fn __builtin_rotateright8([[ARG_0]]: u8, [[ARG_1]]: u8) -> u8;
// CHECK-NEXT:     fn __builtin_rotateleft16([[ARG_0]]: u16, [[ARG_1]]: u16) -> u16;
// CHECK-NEXT:     fn __builtin_rotateright16([[ARG_0]]: u16, [[ARG_1]]: u16) -> u16;
// CHECK-NEXT:     fn __builtin_rotateleft32([[ARG_0]]: u32, [[ARG_1]]: u32) -> u32;
// CHECK-NEXT:     fn __builtin_rotateright32([[ARG_0]]: u32, [[ARG_1]]: u32) -> u32;
// CHECK-NEXT:     fn __builtin_rotateleft64([[ARG_0]]: u64, [[ARG_1]]: u64) -> u64;
// CHECK-NEXT:     fn __builtin_rotateright64([[ARG_0]]: u64, [[ARG_1]]: u64) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u8 = 147u8;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0u32, 1u32, 7u32, 8u32, 9u32, 255u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     8i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left((n as u8) as u32) as u64,
// CHECK-NEXT:                     x.rotate_right((n as u8) as u32) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u16 = 37175u16;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0u32, 1u32, 15u32, 16u32, 17u32, 255u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     16i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left((n as u16) as u32) as u64,
// CHECK-NEXT:                     x.rotate_right((n as u16) as u32) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u32 = 2436344781;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0u32, 1u32, 31u32, 32u32, 33u32, 255u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     32i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left(n as u32) as u64,
// CHECK-NEXT:                     x.rotate_right(n as u32) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u64 = 10464021160185111365;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0u32, 1u32, 63u32, 64u32, 65u32, 255u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     64i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left((n as u64) as u32),
// CHECK-NEXT:                     x.rotate_right((n as u64) as u32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: u32 = 3;
// CHECK-NEXT:     let mut n: u32 = 1;
// CHECK-NEXT:     let mut r: u32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u32 = x;
// CHECK-NEXT:     x = [[V_0]].wrapping_add(1);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = n;
// CHECK-NEXT:     n = [[V_1]].wrapping_add(1);
// CHECK-NEXT:     r = [[V_0]].rotate_left([[V_1]] as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u %u %u\n".as_ptr(), r, x, n) };
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
