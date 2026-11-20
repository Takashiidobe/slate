#include <stdio.h>

static int abs_value = __builtin_abs(-3);
static long labs_value = __builtin_labs(-40L);
static long long llabs_value = __builtin_llabs(-500LL);
static long expected = __builtin_expect(7, 1);
static long expected_probability = __builtin_expect_with_probability(8, 1, 0.9);
static unsigned long length = __builtin_strlen("kernel");
static int ordered = __builtin_strcmp("abc", "abd");
static int same = __builtin_memcmp("abc", "abd", 2);
static double magnitude = __builtin_fabs(-2.5);
static float magnitude_float = __builtin_fabsf(-1.25f);
static double signed_copy = __builtin_copysign(3.0, -0.0);
static double larger = __builtin_fmax(1.5, -7.0);
static double smaller = __builtin_fmin(-1.5, 4.0);
static unsigned long lengths[] = {__builtin_strlen("a"), __builtin_strlen("")};

int runtime_abs(int x) {
    int automatic = __builtin_abs(-9);
    return automatic + __builtin_abs(x);
}

int main(void) {
    printf("%d %ld %lld %ld %ld %lu\n", abs_value, labs_value, llabs_value, expected,
           expected_probability, length);
    printf("%d %d %lu %lu\n", ordered, same, lengths[0], lengths[1]);
    printf("%g %g %g %g %g\n", magnitude, (double)magnitude_float, signed_copy, larger, smaller);
    printf("%d\n", runtime_abs(-4));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abs"]
// CHECK-NEXT:     fn __builtin_abs([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static mut abs_value: i32 = 3;
// CHECK-EMPTY:
// CHECK-NEXT: static mut labs_value: i64 = 40;
// CHECK-EMPTY:
// CHECK-NEXT: static mut llabs_value: i64 = 500;
// CHECK-EMPTY:
// CHECK-NEXT: static mut expected: i64 = 7;
// CHECK-EMPTY:
// CHECK-NEXT: static mut expected_probability: i64 = 8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut length: u64 = 6;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ordered: i32 = -1;
// CHECK-EMPTY:
// CHECK-NEXT: static mut same: i32 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: static mut magnitude: f64 = 2.5f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut magnitude_float: f32 = 1.25f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut signed_copy: f64 = -3.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut larger: f64 = 1.5f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut smaller: f64 = -1.5f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut lengths: __SlateAlign16<[u64; 2]> = __SlateAlign16([1u64, 0u64]);
// CHECK-EMPTY:
// CHECK-NEXT: fn runtime_abs(x: i32) -> i32 {
// CHECK-NEXT:     let automatic: i32 = (-9i32).wrapping_abs();
// CHECK-NEXT:     automatic + x.wrapping_abs()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %ld %lld %ld %ld %lu\n".as_ptr(),
// CHECK-NEXT:             abs_value,
// CHECK-NEXT:             labs_value,
// CHECK-NEXT:             llabs_value,
// CHECK-NEXT:             expected,
// CHECK-NEXT:             expected_probability,
// CHECK-NEXT:             length,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %lu %lu\n".as_ptr(),
// CHECK-NEXT:             ordered,
// CHECK-NEXT:             same,
// CHECK-NEXT:             lengths.0[0],
// CHECK-NEXT:             lengths.0[1],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%g %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             magnitude,
// CHECK-NEXT:             magnitude_float as f64,
// CHECK-NEXT:             signed_copy,
// CHECK-NEXT:             larger,
// CHECK-NEXT:             smaller,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", runtime_abs(-4i32));
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
