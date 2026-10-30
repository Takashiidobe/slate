/* { dg-options "-frounding-math" } */
#include <fenv.h>
#include <stdio.h>

static const int modes[] = {FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO};

int main(void) {
  volatile double one = 1.0, three = 3.0, tiny = 0x1p-60;
  volatile float onef = 1.0f, threef = 3.0f;
  volatile long long big = 0x7ffffffffffffdffll;
  volatile unsigned long long ubig = 0xfffffffffffffbffull;
  volatile int odd = 16777217;
#ifdef __SIZEOF_INT128__
  volatile __int128 wide = ((__int128)1 << 100) + 1;
  volatile unsigned __int128 uwide = ~(unsigned __int128)0 >> 1;
#endif
  for (int m = 0; m < 4; m++) {
    fesetround(modes[m]);
    double folded = 1.0 / 3.0;
    double sum = 1.0 + 0x1p-60;
    double diff = -1.0 - 0x1p-60;
    double prod = 0x1.0000001p0 * 0x1.0000001p0;
    float foldedf = 1.0f / 3.0f;
    double quot = one / three;
    double acc = one;
    acc += tiny;
    acc -= tiny * 3;
    acc *= three;
    acc /= three;
    float quotf = onef / threef;
    float narrow = (float)quot;
    float narrowc = (float)(1.0 / 3.0);
    double fromi = (double)big;
    double fromu = (double)ubig;
    float fromodd = (float)odd;
    float fromi64 = (float)big;
#ifdef __SIZEOF_INT128__
    double fromwide = (double)wide;
    float fromuwide = (float)uwide;
#else
    double fromwide = 0;
    float fromuwide = 0;
#endif
    fesetround(FE_TONEAREST);
    printf("%d %a %a %a %a %a\n", m, folded, sum, diff, prod, (double)foldedf);
    printf("%d %a %a %a %a %a\n", m, quot, acc, (double)quotf, (double)narrow, (double)narrowc);
    printf("%d %a %a %a %a %a %a\n", m, fromi, fromu, (double)fromodd, (double)fromi64, fromwide, (double)fromuwide);
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-frounding-math" }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: mod __slate_rounding {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn pow2_f64(shift: u32) -> f64 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         f64::from_bits((1023 + shift as u64) << 52)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn pow2_f32(shift: u32) -> f32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         f32::from_bits((127 + shift) << 23)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn reduce_i128(x: i128) -> (i64, u32) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let magnitude_bits = 128 - (if x < 0 { !x } else { x }).leading_zeros();
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let shift = magnitude_bits - 62;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let sticky = (x & ((1i128 << shift) - 1) != 0) as i64;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         (((x >> shift) as i64) | sticky, shift)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn reduce_u128(x: u128) -> (i64, u32) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let shift = 128 - x.leading_zeros() - 62;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let sticky = (x & ((1u128 << shift) - 1) != 0) as i64;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         (((x >> shift) as i64) | sticky, shift)
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     pub fn i128_to_f64(x: i128, convert: unsafe extern "C" fn(i64) -> f64) -> f64 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_i128(x);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f64(shift) }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     pub fn i128_to_f32(x: i128, convert: unsafe extern "C" fn(i64) -> f32) -> f32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_i128(x);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f32(shift) }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     pub fn u128_to_f64(x: u128, convert: unsafe extern "C" fn(i64) -> f64) -> f64 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_u128(x);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f64(shift) }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     pub fn u128_to_f32(x: u128, convert: unsafe extern "C" fn(i64) -> f32) -> f32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_u128(x);
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f32(shift) }
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: mod __slate_rounding {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn pow2_f64(shift: u32) -> f64 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         f64::from_bits((1023 + shift as u64) << 52)
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn pow2_f32(shift: u32) -> f32 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         f32::from_bits((127 + shift) << 23)
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn reduce_i128(x: i128) -> (i64, u32) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let magnitude_bits = 128 - (if x < 0 { !x } else { x }).leading_zeros();
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let shift = magnitude_bits - 62;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let sticky = (x & ((1i128 << shift) - 1) != 0) as i64;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         (((x >> shift) as i64) | sticky, shift)
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn reduce_u128(x: u128) -> (i64, u32) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let shift = 128 - x.leading_zeros() - 62;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let sticky = (x & ((1u128 << shift) - 1) != 0) as i64;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         (((x >> shift) as i64) | sticky, shift)
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     pub fn i128_to_f64(x: i128, convert: unsafe extern "C" fn(i64) -> f64) -> f64 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_i128(x);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f64(shift) }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     pub fn i128_to_f32(x: i128, convert: unsafe extern "C" fn(i64) -> f32) -> f32 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_i128(x);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f32(shift) }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     pub fn u128_to_f64(x: u128, convert: unsafe extern "C" fn(i64) -> f64) -> f64 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_u128(x);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f64(shift) }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     pub fn u128_to_f32(x: u128, convert: unsafe extern "C" fn(i64) -> f32) -> f32 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         if let Ok(small) = i64::try_from(x) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             return unsafe { convert(small) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let (reduced, shift) = reduce_u128(x);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { convert(reduced) * pow2_f32(shift) }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     }
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: }
// AARCH64-UNKNOWN-LINUX-GNU-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fesetround([[ARG_0:arg[0-9]+]]: i32) -> i32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0]]: *const u8, ...) -> i32;
// CHECK-NEXT:     fn __slate_fenv_add_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_div_f32(_0: f32, _1: f32) -> f32;
// CHECK-NEXT:     fn __slate_fenv_div_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_f64_to_f32(_0: f64) -> f32;
// CHECK-NEXT:     fn __slate_fenv_i64_to_f32(_0: i64) -> f32;
// CHECK-NEXT:     fn __slate_fenv_i64_to_f64(_0: i64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_mul_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_sub_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_u64_to_f64(_0: u64) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: #[repr(C, align(16))]
// X86_64-UNKNOWN-LINUX-GNU-NEXT: struct __SlateAlign16<T>(T);
// X86_64-UNKNOWN-LINUX-GNU-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: static mut modes: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32, 2048i32, 1024i32, 3072i32]);
// I686-UNKNOWN-LINUX-GNU-NEXT: static mut modes: [i32; 4] = [0, 2048, 1024, 3072];
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: static mut modes: [i32; 4] = [0, 4194304, 8388608, 12582912];
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let one: f64 = 1.0f64;
// CHECK-NEXT:     let three: f64 = 3.0f64;
// CHECK-NEXT:     let tiny: f64 = 8.673617379884035e-19f64;
// CHECK-NEXT:     let onef: f32 = 1.0f32;
// CHECK-NEXT:     let threef: f32 = 3.0f32;
// CHECK-NEXT:     let big: i64 = 9223372036854775295;
// CHECK-NEXT:     let ubig: u64 = 18446744073709550591;
// CHECK-NEXT:     let odd: i32 = 16777217;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let wide: i128 = ((1 as i128) << 100i32) + 1;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let uwide: u128 = !(0 as u128) >> 1i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let wide: i128 = ((1 as i128) << 100i32) + 1;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let uwide: u128 = !(0 as u128) >> 1i32;
// CHECK-NEXT:     for m in 0i32..4i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { fesetround(modes.0[m as usize]) };
// I686-UNKNOWN-LINUX-GNU-NEXT:         unsafe { fesetround(modes[m as usize]) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         unsafe { fesetround(modes[m as usize]) };
// CHECK-NEXT:         let folded: f64 = unsafe { __slate_fenv_div_f64(1.0f64, 3.0f64) };
// CHECK-NEXT:         let sum: f64 = unsafe { __slate_fenv_add_f64(1.0f64, 8.673617379884035e-19f64) };
// CHECK-NEXT:         let diff: f64 = unsafe { __slate_fenv_sub_f64(-1.0f64, 8.673617379884035e-19f64) };
// CHECK-NEXT:         let prod: f64 =
// CHECK-NEXT:             unsafe { __slate_fenv_mul_f64(1.0000000037252903f64, 1.0000000037252903f64) };
// CHECK-NEXT:         let foldedf: f32 = unsafe { __slate_fenv_div_f32(1.0f32, 3.0f32) };
// CHECK-NEXT:         let quot: f64 = unsafe {
// CHECK-NEXT:             __slate_fenv_div_f64(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(one)),
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut acc: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) };
// CHECK-NEXT:         acc =
// CHECK-NEXT:             unsafe { __slate_fenv_add_f64(acc, std::ptr::read_volatile(std::ptr::addr_of!(tiny))) };
// CHECK-NEXT:         acc = unsafe {
// CHECK-NEXT:             __slate_fenv_sub_f64(
// CHECK-NEXT:                 acc,
// CHECK-NEXT:                 __slate_fenv_mul_f64(
// CHECK-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(tiny)),
// CHECK-NEXT:                     3i32 as f64,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         acc = unsafe {
// CHECK-NEXT:             __slate_fenv_mul_f64(acc, std::ptr::read_volatile(std::ptr::addr_of!(three)))
// CHECK-NEXT:         };
// CHECK-NEXT:         acc = unsafe {
// CHECK-NEXT:             __slate_fenv_div_f64(acc, std::ptr::read_volatile(std::ptr::addr_of!(three)))
// CHECK-NEXT:         };
// CHECK-NEXT:         let quotf: f32 = unsafe {
// CHECK-NEXT:             __slate_fenv_div_f32(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(onef)),
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(threef)),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let narrow: f32 = unsafe { __slate_fenv_f64_to_f32(quot) };
// CHECK-NEXT:         let narrowc: f32 = unsafe { __slate_fenv_f64_to_f32(__slate_fenv_div_f64(1.0f64, 3.0f64)) };
// CHECK-NEXT:         let fromi: f64 = unsafe {
// CHECK-NEXT:             __slate_fenv_i64_to_f64(std::ptr::read_volatile(std::ptr::addr_of!(big)) as i64)
// CHECK-NEXT:         };
// CHECK-NEXT:         let fromu: f64 = unsafe {
// CHECK-NEXT:             __slate_fenv_u64_to_f64(std::ptr::read_volatile(std::ptr::addr_of!(ubig)) as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:         let fromodd: f32 = unsafe {
// CHECK-NEXT:             __slate_fenv_i64_to_f32(std::ptr::read_volatile(std::ptr::addr_of!(odd)) as i64)
// CHECK-NEXT:         };
// CHECK-NEXT:         let fromi64: f32 = unsafe {
// CHECK-NEXT:             __slate_fenv_i64_to_f32(std::ptr::read_volatile(std::ptr::addr_of!(big)) as i64)
// CHECK-NEXT:         };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let fromwide: f64 = __slate_rounding::i128_to_f64(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(wide)) },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             __slate_fenv_i64_to_f64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         );
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         let fromuwide: f32 = __slate_rounding::u128_to_f32(
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(uwide)) },
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             __slate_fenv_i64_to_f32,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         );
// I686-UNKNOWN-LINUX-GNU-NEXT:         let fromwide: f64 = 0i32 as f64;
// I686-UNKNOWN-LINUX-GNU-NEXT:         let fromuwide: f32 = unsafe { __slate_fenv_i64_to_f32(0i64) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let fromwide: f64 = __slate_rounding::i128_to_f64(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(wide)) },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             __slate_fenv_i64_to_f64,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         );
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         let fromuwide: f32 = __slate_rounding::u128_to_f32(
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(uwide)) },
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:             __slate_fenv_i64_to_f32,
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:         );
// CHECK-NEXT:         unsafe { fesetround(0i32) };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %a %a %a %a %a\n".as_ptr(),
// CHECK-NEXT:                 m,
// CHECK-NEXT:                 folded,
// CHECK-NEXT:                 sum,
// CHECK-NEXT:                 diff,
// CHECK-NEXT:                 prod,
// CHECK-NEXT:                 foldedf as f64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %a %a %a %a %a\n".as_ptr(),
// CHECK-NEXT:                 m,
// CHECK-NEXT:                 quot,
// CHECK-NEXT:                 acc,
// CHECK-NEXT:                 quotf as f64,
// CHECK-NEXT:                 narrow as f64,
// CHECK-NEXT:                 narrowc as f64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %a %a %a %a %a %a\n".as_ptr(),
// CHECK-NEXT:                 m,
// CHECK-NEXT:                 fromi,
// CHECK-NEXT:                 fromu,
// CHECK-NEXT:                 fromodd as f64,
// CHECK-NEXT:                 fromi64 as f64,
// CHECK-NEXT:                 fromwide,
// CHECK-NEXT:                 fromuwide as f64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
