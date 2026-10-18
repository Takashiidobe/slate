#include <stdint.h>
#include <stdio.h>
#include <string.h>

static float quiet_f = __builtin_nanf("");
static float quiet_payload_f = __builtin_nanf("0x15");
static float signaling_f = __builtin_nansf("");
static float signaling_payload_f = __builtin_nansf("21");
static double quiet_d = __builtin_nan("");
static double quiet_payload_d = __builtin_nan("0123");
static double signaling_d = __builtin_nans("");
static double signaling_payload_d = __builtin_nans("0x123");
static const double table[] = {__builtin_nans("1"), __builtin_nan("2")};

static uint32_t bits_f(float value) {
  uint32_t bits;
  memcpy(&bits, &value, sizeof bits);
  return bits;
}

static uint64_t bits_d(double value) {
  uint64_t bits;
  memcpy(&bits, &value, sizeof bits);
  return bits;
}

int main(void) {
  printf("%08x %08x %08x %08x\n", bits_f(quiet_f), bits_f(quiet_payload_f), bits_f(signaling_f),
         bits_f(signaling_payload_f));
  printf("%016llx %016llx %016llx %016llx\n", (unsigned long long)bits_d(quiet_d),
         (unsigned long long)bits_d(quiet_payload_d), (unsigned long long)bits_d(signaling_d),
         (unsigned long long)bits_d(signaling_payload_d));
  printf("%016llx %016llx\n", (unsigned long long)bits_d(table[0]),
         (unsigned long long)bits_d(table[1]));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut quiet_f: f32 = f32::from_bits(0x7fc00000);
// CHECK-NEXT: static mut quiet_payload_f: f32 = f32::from_bits(0x7fc00015);
// CHECK-NEXT: static mut signaling_f: f32 = f32::from_bits(0x7fa00000);
// CHECK-NEXT: static mut signaling_payload_f: f32 = f32::from_bits(0x7f800015);
// CHECK-NEXT: static mut quiet_d: f64 = f64::from_bits(0x7ff8000000000000);
// CHECK-NEXT: static mut quiet_payload_d: f64 = f64::from_bits(0x7ff8000000000053);
// CHECK-NEXT: static mut signaling_d: f64 = f64::from_bits(0x7ff4000000000000);
// CHECK-NEXT: static mut signaling_payload_d: f64 = f64::from_bits(0x7ff0000000000123);
// CHECK-NEXT: static mut table: __SlateAlign16<[f64; 2]> = __SlateAlign16([
// CHECK-NEXT:     f64::from_bits(0x7ff0000000000001),
// CHECK-NEXT:     f64::from_bits(0x7ff8000000000002),
// CHECK-NEXT: ]);
// CHECK-NEXT: fn bits_f(mut value: f32) -> u32 {
// CHECK-NEXT:     let mut bits: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(value) as *const ()) as *const u8,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(bits) as *mut ()) as *mut u8,
// CHECK-NEXT:             4 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     bits
// CHECK-NEXT: }
// CHECK-NEXT: fn bits_d(mut value: f64) -> u64 {
// CHECK-NEXT:     let mut bits: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(value) as *const ()) as *const u8,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(bits) as *mut ()) as *mut u8,
// CHECK-NEXT:             8 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     bits
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%08x %08x %08x %08x\n".as_ptr(),
// CHECK-NEXT:             bits_f(quiet_f),
// CHECK-NEXT:             bits_f(quiet_payload_f),
// CHECK-NEXT:             bits_f(signaling_f),
// CHECK-NEXT:             bits_f(signaling_payload_f),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%016llx %016llx %016llx %016llx\n".as_ptr(),
// CHECK-NEXT:             bits_d(quiet_d),
// CHECK-NEXT:             bits_d(quiet_payload_d),
// CHECK-NEXT:             bits_d(signaling_d),
// CHECK-NEXT:             bits_d(signaling_payload_d),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%016llx %016llx\n".as_ptr(), bits_d(table.0[0]), bits_d(table.0[1]))
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
