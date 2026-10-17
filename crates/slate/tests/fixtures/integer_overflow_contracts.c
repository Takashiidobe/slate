#include <limits.h>
#include <stdint.h>
#include <stdio.h>

static unsigned scale(unsigned value, unsigned factor) { return value * factor; }

static uint64_t mix(uint64_t state) {
  state ^= state >> 33;
  state *= 0xff51afd7ed558ccdULL;
  state ^= state >> 33;
  state *= 0xc4ceb9fe1a85ec53ULL;
  return state ^ (state >> 33);
}

int main(void) {
  unsigned top = UINT_MAX;
  unsigned zero = 0;
  unsigned long long big = ULLONG_MAX;
  top += 5;
  zero -= 3;
  big *= 3;
  unsigned counter = UINT_MAX;
  counter++;
  unsigned negated = -top;
  printf("%u %u %llu %u %u\n", top, zero, big, counter, negated);
  printf("%u %llu\n", scale(0x80000001u, 6u), (unsigned long long)mix(42));

  int narrow = 0;
  long wide = 0;
  unsigned unsigned_out = 0;
  long long ll_out = 0;
  int a = __builtin_add_overflow(INT_MAX, 1, &wide);
  int b = __builtin_add_overflow(INT_MAX, 1, &narrow);
  int c = __builtin_sub_overflow(0u, 1u, &narrow);
  int d = __builtin_sub_overflow(-1, 0, &unsigned_out);
  int e = __builtin_mul_overflow(UINT_MAX, UINT_MAX, &ll_out);
  int f = __builtin_mul_overflow(LLONG_MIN, -1LL, &ll_out);
  int g = __builtin_mul_overflow(-7, 6, &narrow);
  printf("%d %ld %d %d %d %u\n", a, wide, b, c, d, unsigned_out);
  printf("%d %d %lld %d %d\n", e, f, ll_out, g, narrow);
  if (__builtin_add_overflow(ULLONG_MAX, 2ULL, &unsigned_out)) {
    printf("wrapped %u\n", unsigned_out);
  }
  return (int)(top + zero) & 0x3f;
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
// CHECK-NEXT: fn scale(value: u32, factor: u32) -> u32 {
// CHECK-NEXT:     value.wrapping_mul(factor)
// CHECK-NEXT: }
// CHECK-NEXT: fn mix(mut state: u64) -> u64 {
// CHECK-NEXT:     state ^= state >> (33 as i32);
// CHECK-NEXT:     state = state.wrapping_mul(18397679294719823053);
// CHECK-NEXT:     state ^= state >> (33 as i32);
// CHECK-NEXT:     state = state.wrapping_mul(14181476777654086739);
// CHECK-NEXT:     state ^ state >> (33 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut top: u32 = (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1);
// CHECK-NEXT:     let mut zero: u32 = 0;
// CHECK-NEXT:     let mut big: u64 = (9223372036854775807 as u64)
// CHECK-NEXT:         .wrapping_mul(2 as u64)
// CHECK-NEXT:         .wrapping_add(1);
// CHECK-NEXT:     top = top.wrapping_add(5);
// CHECK-NEXT:     zero = zero.wrapping_sub(3);
// CHECK-NEXT:     big = big.wrapping_mul(3);
// CHECK-NEXT:     let mut counter: u32 = (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1);
// CHECK-NEXT:     counter = counter.wrapping_add(1);
// CHECK-NEXT:     let negated: u32 = top.wrapping_neg();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u %u %llu %u %u\n".as_ptr(), top, zero, big, counter, negated) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%u %llu\n".as_ptr(), scale(2147483649 as u32, 6 as u32), mix(42 as u64))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut narrow: i32 = 0;
// CHECK-NEXT:     let mut wide: i64 = 0;
// CHECK-NEXT:     let mut unsigned_out: u32 = 0;
// CHECK-NEXT:     let mut ll_out: i64 = 0;
// CHECK-NEXT:     let a: i32 = ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = 2147483647 as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(wide) = [[T_2]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as i64) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let b: i32 = ({
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: i128 = 2147483647 as i128;
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: i128 = [[T_3]].wrapping_add([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(narrow) = [[T_5]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_add([[T_4]]).is_none() || (([[T_5]] as i32) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let c: i32 = ({
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: i128 = 0 as i128;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: i128 = [[T_6]].wrapping_sub([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(narrow) = [[T_8]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_sub([[T_7]]).is_none() || (([[T_8]] as i32) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let d: i32 = ({
// CHECK-NEXT:         let [[T_9:__t[0-9]+]]: i128 = -1 as i128;
// CHECK-NEXT:         let [[T_10:__t[0-9]+]]: i128 = 0 as i128;
// CHECK-NEXT:         let [[T_11:__t[0-9]+]]: i128 = [[T_9]].wrapping_sub([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(unsigned_out) = [[T_11]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_9]].checked_sub([[T_10]]).is_none() || (([[T_11]] as u32) as i128) != [[T_11]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let e: i32 = ({
// CHECK-NEXT:         let [[T_12:__t[0-9]+]]: i128 = (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1)
// CHECK-NEXT:             as i128;
// CHECK-NEXT:         let [[T_13:__t[0-9]+]]: i128 = (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1)
// CHECK-NEXT:             as i128;
// CHECK-NEXT:         let [[T_14:__t[0-9]+]]: i128 = [[T_12]].wrapping_mul([[T_13]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ll_out) = [[T_14]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_12]].checked_mul([[T_13]]).is_none() || (([[T_14]] as i64) as i128) != [[T_14]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let f: i32 = ({
// CHECK-NEXT:         let [[T_15:__t[0-9]+]]: i128 = (-(9223372036854775807 as i64) - (1 as i64)) as i128;
// CHECK-NEXT:         let [[T_16:__t[0-9]+]]: i128 = -1 as i128;
// CHECK-NEXT:         let [[T_17:__t[0-9]+]]: i128 = [[T_15]].wrapping_mul([[T_16]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ll_out) = [[T_17]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_15]].checked_mul([[T_16]]).is_none() || (([[T_17]] as i64) as i128) != [[T_17]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let g: i32 = ({
// CHECK-NEXT:         let [[T_18:__t[0-9]+]]: i128 = -7 as i128;
// CHECK-NEXT:         let [[T_19:__t[0-9]+]]: i128 = 6 as i128;
// CHECK-NEXT:         let [[T_20:__t[0-9]+]]: i128 = [[T_18]].wrapping_mul([[T_19]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(narrow) = [[T_20]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_18]].checked_mul([[T_19]]).is_none() || (([[T_20]] as i32) as i128) != [[T_20]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %ld %d %d %d %u\n".as_ptr(), a, wide, b, c, d, unsigned_out) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %d %lld %d %d\n".as_ptr(), e, f, ll_out, g, narrow) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_21:__t[0-9]+]]: i128 = (9223372036854775807 as u64)
// CHECK-NEXT:             .wrapping_mul(2 as u64)
// CHECK-NEXT:             .wrapping_add(1) as i128;
// CHECK-NEXT:         let [[T_22:__t[0-9]+]]: i128 = 2 as i128;
// CHECK-NEXT:         let [[T_23:__t[0-9]+]]: i128 = [[T_21]].wrapping_add([[T_22]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(unsigned_out) = [[T_23]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_21]].checked_add([[T_22]]).is_none() || (([[T_23]] as u32) as i128) != [[T_23]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"wrapped %u\n".as_ptr(), unsigned_out) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (top.wrapping_add(zero) as i32) & 63
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
