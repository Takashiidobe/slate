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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! TEST__2 {
// CHECK-NEXT:     ($T:ty, $MAX:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let mut r : $T = 0i32 as $T; let values :
// CHECK-NEXT:         __SlateAlign16 < {{\[\[}}u32; 2]; 4] > = __SlateAlign16({{\[\[}}0i32 as $T, $MAX], [1i32 as
// CHECK-NEXT:         $T, $MAX], [$MAX, 2i32 as $T], [$MAX, $MAX]]); '__slate_label_2 : for i in 0i32
// CHECK-NEXT:         as $T ..4i32 as $T { let overflow : i32 = ({ let __t : i128 = values.0[i as
// CHECK-NEXT:         usize] [0] as i128; let [[T_0:__t[0-9]+]] : i128 = values.0[i as usize] [1] as i128; let [[T_1:__t[0-9]+]]
// CHECK-NEXT:         : i128 = __t.wrapping_mul([[T_0]]); unsafe { * std::ptr::addr_of_mut!(r) = [[T_1]] as
// CHECK-NEXT:         $T; } __t.checked_mul([[T_0]]).is_none() || (([[T_1]] as $T) as i128) != [[T_1]] }) as
// CHECK-NEXT:         i32; let _ = std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%d %llu\n".as_ptr(), overflow, r as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; } if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! TEST {
// CHECK-NEXT:     ($T:ty, $MAX:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let mut r : $T = 0i32 as $T; let values :
// CHECK-NEXT:         __SlateAlign16 < {{\[\[}}u64; 2]; 4] > = __SlateAlign16({{\[\[}}0i32 as $T, $MAX], [1i32 as
// CHECK-NEXT:         $T, $MAX], [$MAX, 2i32 as $T], [$MAX, $MAX]]); '__slate_label_2 : for i in 0u32
// CHECK-NEXT:         ..4u32 { let overflow : i32 = ({ let __t : i128 = values.0[i as usize] [0] as
// CHECK-NEXT:         i128; let [[T_0]] : i128 = values.0[i as usize] [1] as i128; let [[T_1]] : i128 = __t
// CHECK-NEXT:         .wrapping_mul([[T_0]]); unsafe { * std::ptr::addr_of_mut!(r) = [[T_1]] as $T; } __t
// CHECK-NEXT:         .checked_mul([[T_0]]).is_none() || (([[T_1]] as $T) as i128) != [[T_1]] }) as i32; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%d %llu\n".as_ptr(), overflow, r) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; } if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     TEST__2!(u32, 4294967295u32);
// CHECK-NEXT:     TEST!(u64, 18446744073709551615u64);
// CHECK-NEXT:     TEST!(u64, 18446744073709551615u64);
// CHECK-NEXT:     let mut r: u32 = 0;
// CHECK-NEXT:     let mut overflow: i32 = ({
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = 1u32 as i128;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: i128 = 4294967295u32 as i128;
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: i128 = [[T_2]].wrapping_mul([[T_3]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_4]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].checked_mul([[T_3]]).is_none() || (([[T_4]] as u32) as i128) != [[T_4]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %u\n".as_ptr(), overflow, r) };
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
// CHECK-NEXT:     p = unsafe { [[V_2]].add(1) };
// CHECK-NEXT:     overflow = ({
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: i128 = [[V_0]] as i128;
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: i128 = [[V_1]] as i128;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: i128 = [[T_5]].wrapping_mul([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_2]] = [[T_7]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_5]].checked_mul([[T_6]]).is_none() || (([[T_7]] as u32) as i128) != [[T_7]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %u %u %u %ld\n".as_ptr(),
// CHECK-NEXT:             overflow,
// CHECK-NEXT:             results[0],
// CHECK-NEXT:             a,
// CHECK-NEXT:             b,
// CHECK-NEXT:             p.offset_from(results.as_mut_ptr() as *mut u32) as i64,
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
