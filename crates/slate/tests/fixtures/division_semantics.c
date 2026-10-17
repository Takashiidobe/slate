#include <limits.h>
#include <stdint.h>
#include <stdio.h>

static int quotient(int a, int b) { return a / b; }
static int remainder_of(int a, int b) { return a % b; }

int main(void) {
  int pairs[][2] = {{7, 2}, {-7, 2}, {7, -2}, {-7, -2}, {0, 5}, {INT_MIN, 2}, {INT_MAX, -1}};
  for (unsigned i = 0; i < sizeof pairs / sizeof pairs[0]; i++) {
    int a = pairs[i][0];
    int b = pairs[i][1];
    printf("%d %d %d\n", quotient(a, b), remainder_of(a, b), quotient(a, b) * b + remainder_of(a, b) == a);
  }
  unsigned mixed = -7 / 2u;
  unsigned mixed_rem = -7 % 3u;
  long long big = LLONG_MIN / 3;
  long long big_rem = LLONG_MIN % 7;
  uint64_t ubig = UINT64_MAX / 10;
  uint64_t ubig_rem = UINT64_MAX % 10;
  printf("%u %u %lld %lld %llu %llu\n", mixed, mixed_rem, big, big_rem,
         (unsigned long long)ubig, (unsigned long long)ubig_rem);

  signed char sc = -100;
  unsigned char uc = 250;
  short sh = -30000;
  unsigned short us = 65535;
  sc /= 3;
  uc %= 7;
  sh /= -7;
  us /= 2;
  int8_t narrow = -128;
  narrow /= -1;
  printf("%d %d %d %d %d\n", sc, uc, sh, us, narrow);

  unsigned accumulator = 1000000007u;
  int steps = 0;
  while (accumulator > 1) {
    accumulator = accumulator % 2 ? accumulator / 3 : accumulator / 2;
    steps++;
  }
  printf("%d %d\n", steps, (int)(-17 % 5 + 17 % -5));
  return quotient(-45, 4) & 0xff;
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn quotient(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a / b
// CHECK-NEXT: }
// CHECK-NEXT: fn remainder_of(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a % b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let pairs: __SlateAlign16<{{\[\[}}i32; 2]; 7]> = __SlateAlign16([
// CHECK-NEXT:         [7 as i32, 2 as i32],
// CHECK-NEXT:         [-(7 as i32), 2 as i32],
// CHECK-NEXT:         [7 as i32, -(2 as i32)],
// CHECK-NEXT:         [-(7 as i32), -(2 as i32)],
// CHECK-NEXT:         [0 as i32, 5 as i32],
// CHECK-NEXT:         [-(2147483647 as i32) - (1 as i32), 2 as i32],
// CHECK-NEXT:         [2147483647 as i32, -(1 as i32)],
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 56 / 8 {
// CHECK-NEXT:         let a: i32 = pairs.0[i as usize][0];
// CHECK-NEXT:         let b: i32 = pairs.0[i as usize][1];
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}", quotient(a, b), remainder_of(a, b), (quotient(a, b) * b +
// CHECK-NEXT:             remainder_of(a, b) == a) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mixed: u32 = (-(7 as i32) as u32) / 2;
// CHECK-NEXT:     let mixed_rem: u32 = (-(7 as i32) as u32) % 3;
// CHECK-NEXT:     let big: i64 = (-9223372036854775807 - 1) / 3;
// CHECK-NEXT:     let big_rem: i64 = (-9223372036854775807 - 1) % 7;
// CHECK-NEXT:     let ubig: u64 = 18446744073709551615 / 10;
// CHECK-NEXT:     let ubig_rem: u64 = 18446744073709551615 % 10;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %lld %lld %llu %llu\n".as_ptr(),
// CHECK-NEXT:             mixed,
// CHECK-NEXT:             mixed_rem,
// CHECK-NEXT:             big,
// CHECK-NEXT:             big_rem,
// CHECK-NEXT:             ubig,
// CHECK-NEXT:             ubig_rem,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut sc: i8 = -100;
// CHECK-NEXT:     let mut uc: u8 = ((250 as i32) as i8) as u8;
// CHECK-NEXT:     let mut sh: i16 = -30000;
// CHECK-NEXT:     let mut us: u16 = ((65535 as i32) as i16) as u16;
// CHECK-NEXT:     sc = ((sc as i32) / 3) as i8;
// CHECK-NEXT:     uc = ((((uc as u32) as i32) % 7) as i8) as u8;
// CHECK-NEXT:     sh = ((sh as i32) / -7) as i16;
// CHECK-NEXT:     us = ((((us as u32) as i32) / 2) as i16) as u16;
// CHECK-NEXT:     let mut narrow: i8 = -128;
// CHECK-NEXT:     narrow = ((narrow as i32) / -1) as i8;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", sc as i32, (uc as u32) as i32, sh as i32, (us as u32) as i32,
// CHECK-NEXT:         narrow as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut accumulator: u32 = 1000000007;
// CHECK-NEXT:     let mut steps: i32 = 0;
// CHECK-NEXT:     while accumulator > 1 {
// CHECK-NEXT:         accumulator = if accumulator % 2 != 0 {
// CHECK-NEXT:             accumulator / 3
// CHECK-NEXT:         } else {
// CHECK-NEXT:             accumulator / 2
// CHECK-NEXT:         };
// CHECK-NEXT:         steps += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", steps, - 17 % 5 + 17 % - 5);
// CHECK-NEXT:     quotient(-(45 as i32), 4 as i32) & 255
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
