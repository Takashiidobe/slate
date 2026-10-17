#include <math.h>
#include <stdio.h>
static int calls;
static const char *payload(void) { ++calls; return "0x23"; }
int main(void) {
    volatile float infinity = INFINITY;
    volatile double huge = __builtin_inf();
    volatile float nan = NAN;
    union { float value; unsigned int bits; } f = {__builtin_nanf("0x123")};
    union { double value; unsigned long long bits; } d = {__builtin_nan(payload())};
    printf("%d %d %d %08x %016llx %d\n", !!isinf(infinity), !!isinf(huge), !!isnan(nan), f.bits, d.bits, calls);
    return calls != 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn nan(__tagb: *const i8) -> f64;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_inff() -> f32;
// CHECK-NEXT:     fn __builtin_inf() -> f64;
// CHECK-NEXT:     #[link_name = "nan"]
// CHECK-NEXT:     fn __builtin_nan([[V_0:__v[0-9]+]]: *const i8) -> f64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     value: f32,
// CHECK-NEXT:     bits: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     value: f64,
// CHECK-NEXT:     bits: u64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn payload() -> *const i8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     c"0x23".as_ptr()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let infinity: f32 = f32::INFINITY;
// CHECK-NEXT:     let huge: f64 = f64::INFINITY;
// CHECK-NEXT:     let nan: f32 = f32::from_bits(0x7fc00000);
// CHECK-NEXT:     let f: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = f32::from_bits(0x7fc00123);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let d: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].value = unsafe { __builtin_nan(payload()) };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %08x %016llx %d\n".as_ptr(),
// CHECK-NEXT:             !(!((if unsafe { std::ptr::read_volatile(std::ptr::addr_of!(infinity)) }
// CHECK-NEXT:                 .is_infinite()
// CHECK-NEXT:             {
// CHECK-NEXT:                 if unsafe { std::ptr::read_volatile(std::ptr::addr_of!(infinity)) }
// CHECK-NEXT:                     .is_sign_negative()
// CHECK-NEXT:                 {
// CHECK-NEXT:                     -1 as i32
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     1 as i32
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0 as i32
// CHECK-NEXT:             }) != 0)) as i32,
// CHECK-NEXT:             !(!((if unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) }
// CHECK-NEXT:                 .is_infinite()
// CHECK-NEXT:             {
// CHECK-NEXT:                 if unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) }
// CHECK-NEXT:                     .is_sign_negative()
// CHECK-NEXT:                 {
// CHECK-NEXT:                     -1 as i32
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     1 as i32
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0 as i32
// CHECK-NEXT:             }) != 0)) as i32,
// CHECK-NEXT:             !(!unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nan)) }.is_nan())
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             unsafe { f.bits },
// CHECK-NEXT:             unsafe { d.bits },
// CHECK-NEXT:             unsafe { calls },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     ((unsafe { calls }) != 1) as i32
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
