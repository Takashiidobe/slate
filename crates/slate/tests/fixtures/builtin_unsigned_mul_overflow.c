#include <stdio.h>
#include <limits.h>

#define TEST(NAME, T, MAX) do { \
  T r = 0; \
  T values[][2] = {{0, MAX}, {1, MAX}, {MAX, 2}, {MAX, MAX}}; \
  for (unsigned i = 0; i < 4; ++i) { \
    int overflow = NAME(values[i][0], values[i][1], &r); \
    printf("%d %llu\n", overflow, (unsigned long long)r); \
  } \
} while (0)

int main(void) {
  TEST(__builtin_umul_overflow, unsigned, UINT_MAX);
  TEST(__builtin_umull_overflow, unsigned long, ULONG_MAX);
  TEST(__builtin_umulll_overflow, unsigned long long, ULLONG_MAX);
  unsigned r;
  int overflow = __builtin_umul_overflow(0x100000001ULL, -1, &r);
  printf("%d %u\n", overflow, r);
  unsigned a = 3, b = 4, results[2] = {0, 0};
  unsigned *p = results;
  overflow = __builtin_umul_overflow(a++, b++, p++);
  printf("%d %u %u %u %ld\n", overflow, results[0], a, b, (long)(p - results));
  return 0;
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut r: u32 = 0;
// CHECK-NEXT:         let values: __SlateAlign16<{{\[\[}}u32; 2]; 4]> = __SlateAlign16([
// CHECK-NEXT:             [0 as u32, (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1)],
// CHECK-NEXT:             [1 as u32, (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1)],
// CHECK-NEXT:             [(2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1), 2 as u32],
// CHECK-NEXT:             [
// CHECK-NEXT:                 (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1),
// CHECK-NEXT:                 (2147483647 as u32).wrapping_mul(2 as u32).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:         ]);
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             let overflow: i32 = ({
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]]: i128 = values.0[i as usize][0] as i128;
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]]: i128 = values.0[i as usize][1] as i128;
// CHECK-NEXT:                 let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *std::ptr::addr_of_mut!(r) = [[T_2]] as u32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:             }) as i32;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { printf(c"%d %llu\n".as_ptr(), overflow, r as u64) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut r: u64 = 0;
// CHECK-NEXT:         let values: __SlateAlign16<{{\[\[}}u64; 2]; 4]> = __SlateAlign16([
// CHECK-NEXT:             [
// CHECK-NEXT:                 0 as u64,
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:             [
// CHECK-NEXT:                 1 as u64,
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:             [
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:                 2 as u64,
// CHECK-NEXT:             ],
// CHECK-NEXT:             [
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:         ]);
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             let overflow: i32 = ({
// CHECK-NEXT:                 let [[T_3:__t[0-9]+]]: i128 = values.0[i as usize][0] as i128;
// CHECK-NEXT:                 let [[T_4:__t[0-9]+]]: i128 = values.0[i as usize][1] as i128;
// CHECK-NEXT:                 let [[T_5:__t[0-9]+]]: i128 = [[T_3]].wrapping_mul([[T_4]]);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *std::ptr::addr_of_mut!(r) = [[T_5]] as u64;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 [[T_3]].checked_mul([[T_4]]).is_none() || (([[T_5]] as u64) as i128) != [[T_5]]
// CHECK-NEXT:             }) as i32;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { printf(c"%d %llu\n".as_ptr(), overflow, r) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut r: u64 = 0;
// CHECK-NEXT:         let values: __SlateAlign16<{{\[\[}}u64; 2]; 4]> = __SlateAlign16([
// CHECK-NEXT:             [
// CHECK-NEXT:                 0 as u64,
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:             [
// CHECK-NEXT:                 1 as u64,
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:             [
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:                 2 as u64,
// CHECK-NEXT:             ],
// CHECK-NEXT:             [
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:                 (9223372036854775807 as u64).wrapping_mul(2 as u64).wrapping_add(1),
// CHECK-NEXT:             ],
// CHECK-NEXT:         ]);
// CHECK-NEXT:         let mut i: u32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             let overflow: i32 = ({
// CHECK-NEXT:                 let [[T_6:__t[0-9]+]]: i128 = values.0[i as usize][0] as i128;
// CHECK-NEXT:                 let [[T_7:__t[0-9]+]]: i128 = values.0[i as usize][1] as i128;
// CHECK-NEXT:                 let [[T_8:__t[0-9]+]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *std::ptr::addr_of_mut!(r) = [[T_8]] as u64;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 [[T_6]].checked_mul([[T_7]]).is_none() || (([[T_8]] as u64) as i128) != [[T_8]]
// CHECK-NEXT:             }) as i32;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { printf(c"%d %llu\n".as_ptr(), overflow, r) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut r: u32 = 0;
// CHECK-NEXT:     let mut overflow: i32 = ({
// CHECK-NEXT:         let [[T_9:__t[0-9]+]]: i128 = ((4294967297 as u64) as u32) as i128;
// CHECK-NEXT:         let [[T_10:__t[0-9]+]]: i128 = (-(1 as i32) as u32) as i128;
// CHECK-NEXT:         let [[T_11:__t[0-9]+]]: i128 = [[T_9]].wrapping_mul([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_11]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_9]].checked_mul([[T_10]]).is_none() || (([[T_11]] as u32) as i128) != [[T_11]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %u\n".as_ptr(), overflow, r) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut a: u32 = 3;
// CHECK-NEXT:     let mut b: u32 = 4;
// CHECK-NEXT:     let mut results: [u32; 2] = [0, 0];
// CHECK-NEXT:     let mut p: *mut u32 = results.as_mut_ptr() as *mut u32;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u32 = a;
// CHECK-NEXT:     a = [[V_0]].wrapping_add(1);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u32 = b;
// CHECK-NEXT:     b = [[V_1]].wrapping_add(1);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut u32 = p;
// CHECK-NEXT:     p = unsafe { [[V_2]].offset(1 as isize) };
// CHECK-NEXT:     overflow = ({
// CHECK-NEXT:         let [[T_12:__t[0-9]+]]: i128 = [[V_0]] as i128;
// CHECK-NEXT:         let [[T_13:__t[0-9]+]]: i128 = [[V_1]] as i128;
// CHECK-NEXT:         let [[T_14:__t[0-9]+]]: i128 = [[T_12]].wrapping_mul([[T_13]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_2]] = [[T_14]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_12]].checked_mul([[T_13]]).is_none() || (([[T_14]] as u32) as i128) != [[T_14]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %u %u %u %ld\n".as_ptr(),
// CHECK-NEXT:             overflow,
// CHECK-NEXT:             results[0],
// CHECK-NEXT:             a,
// CHECK-NEXT:             b,
// CHECK-NEXT:             (unsafe { p.offset_from(results.as_mut_ptr() as *mut u32) }) as i64,
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
