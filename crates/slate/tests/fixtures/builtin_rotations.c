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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_rotateleft8([[V_0:__v[0-9]+]]: u8, [[V_1:__v[0-9]+]]: u8) -> u8;
// CHECK-NEXT:     fn __builtin_rotateright8([[V_2:__v[0-9]+]]: u8, [[V_3:__v[0-9]+]]: u8) -> u8;
// CHECK-NEXT:     fn __builtin_rotateleft16([[V_4:__v[0-9]+]]: u16, [[V_5:__v[0-9]+]]: u16) -> u16;
// CHECK-NEXT:     fn __builtin_rotateright16([[V_6:__v[0-9]+]]: u16, [[V_7:__v[0-9]+]]: u16) -> u16;
// CHECK-NEXT:     fn __builtin_rotateleft32([[V_8:__v[0-9]+]]: u32, [[V_9:__v[0-9]+]]: u32) -> u32;
// CHECK-NEXT:     fn __builtin_rotateright32([[V_10:__v[0-9]+]]: u32, [[V_11:__v[0-9]+]]: u32) -> u32;
// CHECK-NEXT:     fn __builtin_rotateleft64([[V_12:__v[0-9]+]]: u64, [[V_13:__v[0-9]+]]: u64) -> u64;
// CHECK-NEXT:     fn __builtin_rotateright64([[V_14:__v[0-9]+]]: u64, [[V_15:__v[0-9]+]]: u64) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u8 = ((147 as i32) as i8) as u8;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0 as u32,
// CHECK-NEXT:             1 as u32,
// CHECK-NEXT:             ((8 as i32) - (1 as i32)) as u32,
// CHECK-NEXT:             8 as u32,
// CHECK-NEXT:             ((8 as i32) + (1 as i32)) as u32,
// CHECK-NEXT:             255 as u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     8 as i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left((n as u8) as u32) as u64,
// CHECK-NEXT:                     x.rotate_right((n as u8) as u32) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u16 = ((37175 as i32) as i16) as u16;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0 as u32,
// CHECK-NEXT:             1 as u32,
// CHECK-NEXT:             ((16 as i32) - (1 as i32)) as u32,
// CHECK-NEXT:             16 as u32,
// CHECK-NEXT:             ((16 as i32) + (1 as i32)) as u32,
// CHECK-NEXT:             255 as u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     16 as i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left((n as u16) as u32) as u64,
// CHECK-NEXT:                     x.rotate_right((n as u16) as u32) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u32 = 2436344781;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0 as u32,
// CHECK-NEXT:             1 as u32,
// CHECK-NEXT:             ((32 as i32) - (1 as i32)) as u32,
// CHECK-NEXT:             32 as u32,
// CHECK-NEXT:             ((32 as i32) + (1 as i32)) as u32,
// CHECK-NEXT:             255 as u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     32 as i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left(n as u32) as u64,
// CHECK-NEXT:                     x.rotate_right(n as u32) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let x: u64 = 10464021160185111365;
// CHECK-NEXT:         let counts: __SlateAlign16<[u32; 6]> = __SlateAlign16([
// CHECK-NEXT:             0 as u32,
// CHECK-NEXT:             1 as u32,
// CHECK-NEXT:             ((64 as i32) - (1 as i32)) as u32,
// CHECK-NEXT:             64 as u32,
// CHECK-NEXT:             ((64 as i32) + (1 as i32)) as u32,
// CHECK-NEXT:             255 as u32,
// CHECK-NEXT:         ]);
// CHECK-NEXT:         for i in counts.0.iter().copied() {
// CHECK-NEXT:             let n: u32 = i;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     64 as i32,
// CHECK-NEXT:                     n,
// CHECK-NEXT:                     x.rotate_left((n as u64) as u32),
// CHECK-NEXT:                     x.rotate_right((n as u64) as u32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: u32 = 3;
// CHECK-NEXT:     let mut n: u32 = 1;
// CHECK-NEXT:     let mut r: u32 = 0;
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: u32 = x;
// CHECK-NEXT:     x = [[V_16]].wrapping_add(1);
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: u32 = n;
// CHECK-NEXT:     n = [[V_17]].wrapping_add(1);
// CHECK-NEXT:     r = [[V_16]].rotate_left([[V_17]] as u32);
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
