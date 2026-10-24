#include <math.h>
#include <float.h>
#include <stdio.h>

static int calls;
static double once(double x) { ++calls; return x; }
int main(void) {
    union { unsigned long long bits; double value; } inf = {0x7ff0000000000000ULL}, nan = {0x7ff8000000000001ULL};
    union { unsigned int bits; float value; } finf = {0x7f800000U}, fnan = {0x7fc00001U};
    volatile double values[] = {0.0, -0.0, 1.0, -2.0, DBL_MIN / 2, inf.value, -inf.value, nan.value, -nan.value};
    for (int i = 0; i < 9; ++i) {
        double x = values[i];
        printf("%d%d%d%d%d %d\n", !!isnan(x), !!isinf(x), !!isfinite(x), !!isnormal(x), !!signbit(x), fpclassify(x));
    }
    volatile float small[] = {0.0f, -0.0f, 1.0f, FLT_MIN / 2, finf.value, fnan.value};
    for (int i = 0; i < 6; ++i) {
        float x = small[i];
        printf("%d%d%d%d%d %d\n", !!isnan(x), !!isinf(x), !!isfinite(x), !!isnormal(x), !!signbit(x), fpclassify(x));
    }
    int result = isnan(once(nan.value));
    printf("%d %d\n", !!result, calls);
    return calls != 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn nan(__tagb: *const i8) -> f64;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     bits: u64,
// CHECK-NEXT:     value: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     bits: u32,
// CHECK-NEXT:     value: f32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn once(x: f64) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let inf: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].bits = 9218868437227405312u64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let nan: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].bits = 9221120237041090561u64;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let finf: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].bits = 2139095040u32;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let fnan: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]].bits = 2143289345u32;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut values: __SlateAlign16<[f64; 9]> = __SlateAlign16([
// CHECK-NEXT:         0.0f64,
// CHECK-NEXT:         -0.0f64,
// CHECK-NEXT:         1.0f64,
// CHECK-NEXT:         -2.0f64,
// CHECK-NEXT:         2.2250738585072014e-308f64 / (2i32 as f64),
// CHECK-NEXT:         unsafe { inf.value },
// CHECK-NEXT:         -unsafe { inf.value },
// CHECK-NEXT:         unsafe { nan.value },
// CHECK-NEXT:         -unsafe { nan.value },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0i32..9i32 {
// CHECK-NEXT:         let x: f64 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                 (values.0.as_mut_ptr() as *mut f64).offset(i as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{}{}{}{}{} {}", ! (! x.is_nan()) as i32, ((if x.is_infinite() { if x
// CHECK-NEXT:             .is_sign_negative() { - 1i32 } else { 1i32 } } else { 0i32 }) != 0) as i32, !
// CHECK-NEXT:             (! x.is_finite()) as i32, ! (! x.is_normal()) as i32, ! (! x
// CHECK-NEXT:             .is_sign_negative()) as i32, if x.is_nan() { 0i32 } else { if x.is_infinite()
// CHECK-NEXT:             { 1i32 } else { if x.is_normal() { 4i32 } else { if x.is_subnormal() { 3i32 }
// CHECK-NEXT:             else { 2i32 } } } }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut small: __SlateAlign16<[f32; 6]> = __SlateAlign16([
// CHECK-NEXT:         0.0f32,
// CHECK-NEXT:         -0.0f32,
// CHECK-NEXT:         1.0f32,
// CHECK-NEXT:         1.1754944e-38f32 / (2i32 as f32),
// CHECK-NEXT:         unsafe { finf.value },
// CHECK-NEXT:         unsafe { fnan.value },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0i32..6i32 {
// CHECK-NEXT:         let x: f32 = unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(
// CHECK-NEXT:                 (small.0.as_mut_ptr() as *mut f32).offset(i as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{}{}{}{}{} {}", ! (! x.is_nan()) as i32, ((if x.is_infinite() { if x
// CHECK-NEXT:             .is_sign_negative() { - 1i32 } else { 1i32 } } else { 0i32 }) != 0) as i32, !
// CHECK-NEXT:             (! x.is_finite()) as i32, ! (! x.is_normal()) as i32, ! (! x
// CHECK-NEXT:             .is_sign_negative()) as i32, if x.is_nan() { 0i32 } else { if x.is_infinite()
// CHECK-NEXT:             { 1i32 } else { if x.is_normal() { 4i32 } else { if x.is_subnormal() { 3i32 }
// CHECK-NEXT:             else { 2i32 } } } }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let result: i32 = once(unsafe { nan.value }).is_nan() as i32;
// CHECK-NEXT:     println!("{} {}", (result != 0) as i32, unsafe { calls });
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
