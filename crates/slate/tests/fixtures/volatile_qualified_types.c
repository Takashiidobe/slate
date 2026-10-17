#include <stdio.h>

static volatile char   marker = 65;
static volatile double gain   = 1.5;

struct VolatileFields {
  volatile int    count;
  volatile double ratio;
};

static volatile int bump_return(int value) { return value + 1; }

static double read_volatile_param(volatile double value) { return value + 0.5; }

static double use_volatile_fields(double input) {
  struct VolatileFields fields;
  fields.count = bump_return(4);
  fields.ratio = input + gain;
  return fields.ratio + fields.count;
}

int main(void) {
  marker = marker + 1;
  gain   = read_volatile_param(gain);
  printf("%c\n", marker);
  printf("%f\n", gain);
  printf("%f\n", use_volatile_fields(2.0));
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
// CHECK-NEXT: static mut marker: i8 = (65 as i32) as i8;
// CHECK-NEXT: static mut gain: f64 = 1.5f64;
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct VolatileFields {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     ratio: f64,
// CHECK-NEXT: }
// CHECK-NEXT: fn bump_return(value: i32) -> i32 {
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn read_volatile_param(value: f64) -> f64 {
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(value)) }) + 0.5f64
// CHECK-NEXT: }
// CHECK-NEXT: fn use_volatile_fields(input: f64) -> f64 {
// CHECK-NEXT:     let mut fields: VolatileFields = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(fields.count),
// CHECK-NEXT:             bump_return(4 as i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(fields.ratio),
// CHECK-NEXT:             input + unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.ratio)) })
// CHECK-NEXT:         + ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fields.count)) }) as f64)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(marker),
// CHECK-NEXT:             (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) }) as i32)
// CHECK-NEXT:                 + 1) as i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(gain),
// CHECK-NEXT:             read_volatile_param(unsafe {
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(gain))
// CHECK-NEXT:             }),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c\n".as_ptr(),
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(marker)) }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%f\n".as_ptr(),
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(gain)) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), use_volatile_fields(2.0f64)) };
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
