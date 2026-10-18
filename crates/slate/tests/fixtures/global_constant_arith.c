#include <stdio.h>

static int negative          = -1;
static long long wide_neg    = -9000000000LL;
static unsigned wrapped      = -1u;
static int mixed             = (3 + 4) * -2 - 10 / 3 % 2;
static int bits              = (0xf0 | 0x0f) & ~0x3 ^ 1 << 4;
static unsigned shifted      = 0x80000000u >> 31;
static double scaled         = -2.5 * 4.0 + 1.0;
static float halved          = -(1.0f / 2.0f);
static char sign             = -'a';
static int table[3]          = {-1, -2 * 3, 7 - 10};
static struct { int lo, hi; } range = {-5, -(-5)};
int exported                 = -42;

int main(void) {
  printf("%d %lld %u %d %d %u\n", negative, wide_neg, wrapped, mixed, bits, shifted);
  printf("%g %g %d\n", scaled, halved, sign);
  printf("%d %d %d %d %d %d\n", table[0], table[1], table[2], range.lo, range.hi, exported);
  negative--;
  printf("%d\n", negative);
  return 0;
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
// CHECK-NEXT: static mut negative: i32 = -(1 as i32);
// CHECK-NEXT: static mut wide_neg: i64 = -(9000000000 as i64);
// CHECK-NEXT: static mut wrapped: u32 = (1 as u32).wrapping_neg();
// CHECK-NEXT: static mut mixed: i32 = ((3 as i32) + (4 as i32)) * -(2 as i32)
// CHECK-NEXT:     - (10 as i32) / (3 as i32) % (2 as i32);
// CHECK-NEXT: static mut bits: i32 = ((240 as i32) | (15 as i32)) & !(3 as i32)
// CHECK-NEXT:     ^ (1 as i32) << (4 as i32);
// CHECK-NEXT: static mut shifted: u32 = (2147483648 as u32) >> (31 as i32);
// CHECK-NEXT: static mut scaled: f64 = -2.5f64 * 4.0f64 + 1.0f64;
// CHECK-NEXT: static mut halved: f32 = -(1.0f32 / 2.0f32);
// CHECK-NEXT: static mut sign: i8 = -(97 as i32) as i8;
// CHECK-NEXT: static mut table: [i32; 3] = [
// CHECK-NEXT:     -(1 as i32),
// CHECK-NEXT:     -(2 as i32) * (3 as i32),
// CHECK-NEXT:     (7 as i32) - (10 as i32),
// CHECK-NEXT: ];
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     lo: i32,
// CHECK-NEXT:     hi: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut range: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     lo: -(5 as i32),
// CHECK-NEXT:     hi: -(-(5 as i32)),
// CHECK-NEXT: };
// CHECK-NEXT: static mut exported: i32 = -(42 as i32);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %lld %u %d %d %u\n".as_ptr(),
// CHECK-NEXT:             negative,
// CHECK-NEXT:             wide_neg,
// CHECK-NEXT:             wrapped,
// CHECK-NEXT:             mixed,
// CHECK-NEXT:             bits,
// CHECK-NEXT:             shifted,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g %d\n".as_ptr(), scaled, halved as f64, sign as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", unsafe { table[0] }, unsafe { table[1] }, unsafe { table[2]
// CHECK-NEXT:         }, unsafe { range.lo }, unsafe { range.hi }, unsafe { exported }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         negative = negative - 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { negative });
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
