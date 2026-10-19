#include <stdio.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

static int calls;

static int next(void) { return ++calls; }

static long weigh(const unsigned long long *words, int count) {
  long total = 0;
  for (int i = 0; i < count; i++) {
    __builtin_prefetch(words + i + 4);
    __builtin_prefetch(words + i + 8, 0, 3);
    unsigned long long word = words[i];
    if (unlikely(word == 0)) continue;
    total += __builtin_popcountll(word) * 100 + __builtin_ctzll(word);
    if (likely(word & 1)) total += __builtin_popcountl((unsigned long)(word >> 32));
    total += __builtin_ctz((unsigned)word | 0x80000000u) + __builtin_popcount((unsigned)word);
    total += __builtin_ctzl((unsigned long)word);
  }
  return total;
}

int main(void) {
  unsigned long long words[12] = {
      0, 1, 0x8000000000000000ull, 0xffffffffffffffffull, 0x00f0f0f0f0f0f000ull, 12345,
      0x9e3779b97f4a7c15ull, 2, 0x100000000ull, 0x7fffffffull, 6, 0x5555555555555555ull,
  };
  printf("%ld\n", weigh(words, 12));
  long e = __builtin_expect(next(), 1) + __builtin_expect(next() * 10, 20);
  printf("%ld %d\n", e, calls);
  if (__builtin_expect_with_probability(calls == 2, 1, 0.9)) printf("probable\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_prefetch([[V_0:__v[0-9]+]]: *const (), ...);
// CHECK-NEXT:     fn __builtin_expect([[V_1:__v[0-9]+]]: i64, [[V_2:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     fn __builtin_expect_with_probability([[V_3:__v[0-9]+]]: i64, [[V_4:__v[0-9]+]]: i64, [[V_5:__v[0-9]+]]: f64) -> i64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn next() -> i32 {
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = (unsafe { calls }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_6]]
// CHECK-NEXT: }
// CHECK-NEXT: fn weigh(words: &[u64]) -> i64 {
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < (words.len() as i32) {
// CHECK-NEXT:         let word: u64 = unsafe { words[i as usize] };
// CHECK-NEXT:         if (!(!(word == 0)) as i64) == 0 {
// CHECK-NEXT:             total
// CHECK-NEXT:                 += ((word.count_ones() as i32) * 100 + (word.trailing_zeros() as i32))
// CHECK-NEXT:                     as i64;
// CHECK-NEXT:             if (!(!(word & 1 != 0)) as i64) != 0 {
// CHECK-NEXT:                 total += ((word >> 32i32).count_ones() as i32) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             total
// CHECK-NEXT:                 += ((((word as u32) | 2147483648).trailing_zeros() as i32)
// CHECK-NEXT:                     + ((word as u32).count_ones() as i32)) as i64;
// CHECK-NEXT:             total += (word.trailing_zeros() as i32) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let words: __SlateAlign16<[u64; 12]> = __SlateAlign16([
// CHECK-NEXT:         0u64, 1u64, 9223372036854775808u64, 18446744073709551615u64,
// CHECK-NEXT:         67818912035696640u64, 12345u64, 11400714819323198485u64, 2u64, 4294967296u64,
// CHECK-NEXT:         2147483647u64, 6u64, 6148914691236517205u64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), weigh(&words.0)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let e: i64 = (next() as i64) + ((next() * 10) as i64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld %d\n".as_ptr(), e, calls) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if (((unsafe { calls }) == 2) as i64) != 0 {
// CHECK-NEXT:         println!("probable");
// CHECK-NEXT:     }
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
