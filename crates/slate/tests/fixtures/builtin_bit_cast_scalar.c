#include <stdint.h>
#include <stdio.h>

static float f32_from(uint32_t bits) { return __builtin_bit_cast(float, bits); }
static uint32_t f32_bits(float value) { return __builtin_bit_cast(uint32_t, value); }
static double f64_from(int64_t bits) { return __builtin_bit_cast(double, bits); }
static int64_t f64_bits(double value) { return __builtin_bit_cast(int64_t, value); }

int main(void) {
  volatile uint32_t one = 0x3f800000u;
  volatile float half = -0.5f;
  volatile int64_t two = 0x4000000000000000ll;
  volatile double tiny = 4.9406564584124654e-324;
  printf("%g %08x\n", f32_from(one), f32_bits(half));
  printf("%g %016llx\n", f64_from(two), (unsigned long long)f64_bits(tiny));
  printf("%08x %d\n", f32_bits(f32_from(0x7fc00001u)), f32_from(0x7f800000u) > 1e38f);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f32_from(bits: u32) -> f32 {
// CHECK-NEXT:     unsafe { core::mem::transmute::<u32, f32>(bits) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f32_bits(value: f32) -> u32 {
// CHECK-NEXT:     unsafe { core::mem::transmute::<f32, u32>(value) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f64_from(bits: i64) -> f64 {
// CHECK-NEXT:     unsafe { core::mem::transmute::<i64, f64>(bits) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f64_bits(value: f64) -> i64 {
// CHECK-NEXT:     unsafe { core::mem::transmute::<f64, i64>(value) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let one: u32 = 1065353216;
// CHECK-NEXT:     let half: f32 = -0.5f32;
// CHECK-NEXT:     let two: i64 = 4611686018427387904;
// CHECK-NEXT:     let tiny: f64 = 5e-324f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%g %08x\n".as_ptr(),
// CHECK-NEXT:             f32_from(core::ptr::read_volatile(core::ptr::addr_of!(one))) as f64,
// CHECK-NEXT:             f32_bits(core::ptr::read_volatile(core::ptr::addr_of!(half))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%g %016llx\n".as_ptr(),
// CHECK-NEXT:             f64_from(core::ptr::read_volatile(core::ptr::addr_of!(two))),
// CHECK-NEXT:             f64_bits(core::ptr::read_volatile(core::ptr::addr_of!(tiny))) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%08x %d\n".as_ptr(),
// CHECK-NEXT:             f32_bits(f32_from(2143289345u32)),
// CHECK-NEXT:             (f32_from(2139095040u32) > 1e38f32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
