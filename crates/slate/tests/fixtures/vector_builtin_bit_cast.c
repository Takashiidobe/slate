#include <stdio.h>
#include <string.h>

typedef long long v2i64 __attribute__((vector_size(16)));
typedef double v2f64 __attribute__((vector_size(16)));
typedef float v4f32 __attribute__((vector_size(16)));
typedef unsigned char v16u8 __attribute__((vector_size(16)));
typedef int v2i32 __attribute__((vector_size(8)));
typedef short v4i16 __attribute__((vector_size(8)));

struct pair {
    int low;
    int high;
};

static v2f64 as_doubles(v2i64 bits) {
    return __builtin_bit_cast(v2f64, bits);
}

static v16u8 as_bytes(v4f32 value) {
    return __builtin_bit_cast(v16u8, value);
}

int main(void) {
    v2i64 bits = {0x3ff0000000000000LL, (long long)0xc000000000000000ULL};
    v2f64 doubles = as_doubles(bits);
    printf("%g %g\n", doubles[0], doubles[1]);

    v2i64 back = __builtin_bit_cast(v2i64, doubles);
    printf("%llx %llx\n", (unsigned long long)back[0], (unsigned long long)back[1]);

    v4f32 floats = {1.0f, -0.0f, 2.5f, -3.75f};
    v16u8 bytes = as_bytes(floats);
    for (int i = 0; i < 16; i++)
        printf("%02x", bytes[i]);
    printf("\n");

    double scalar = __builtin_bit_cast(double, (v2i32){0, 0x40080000});
    printf("%g\n", scalar);

    v4i16 halves = __builtin_bit_cast(v4i16, 0x0004000300020001LL);
    printf("%d %d %d %d\n", halves[0], halves[1], halves[2], halves[3]);

    long long whole = __builtin_bit_cast(long long, halves);
    printf("%llx\n", (unsigned long long)whole);

    struct pair pair = __builtin_bit_cast(struct pair, (v2i32){-7, 9});
    printf("%d %d\n", pair.low, pair.high);

    v2i32 packed = __builtin_bit_cast(v2i32, pair);
    printf("%d %d\n", packed[0], packed[1]);

    unsigned int words[4];
    memcpy(words, &floats, sizeof words);
    v4f32 rebuilt = __builtin_bit_cast(v4f32, __builtin_bit_cast(v16u8, floats));
    printf("%d\n", memcmp(&rebuilt, words, sizeof words) == 0);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     low: i32,
// CHECK-NEXT:     high: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn as_doubles(bits: std::simd::Simd<i64, 2>) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<f64, 2>>(bits)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn as_bytes(value: std::simd::Simd<f32, 4>) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<f32, 4>, std::simd::Simd<u8, 16>>(value)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let bits: std::simd::Simd<i64, 2> = std::simd::Simd::from_array([
// CHECK-NEXT:         4607182418800017408i64,
// CHECK-NEXT:         -4611686018427387904i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let doubles: std::simd::Simd<f64, 2> = as_doubles(bits);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%g %g\n".as_ptr(), doubles[0], doubles[1]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let back: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<f64, 2>, std::simd::Simd<i64, 2>>(doubles)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%llx %llx\n".as_ptr(), back[0] as u64, back[1] as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut floats: std::simd::Simd<f32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         1.0f32,
// CHECK-NEXT:         -0.0f32,
// CHECK-NEXT:         2.5f32,
// CHECK-NEXT:         -3.75f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let bytes: std::simd::Simd<u8, 16> = as_bytes(floats);
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%02x".as_ptr(), bytes[i as usize] as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let scalar: f64 = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 2>,
// CHECK-NEXT:             f64,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([0i32, 1074266112i32]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%g\n".as_ptr(), scalar) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let halves: std::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<i64, std::simd::Simd<i16, 4>>(1125912791875585i64)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", halves[0] as i32, halves[1] as i32, halves[2] as i32, halves[3] as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let whole: i64 = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i16, 4>, i64>(halves)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%llx\n".as_ptr(), whole as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let pair: pair = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 2>,
// CHECK-NEXT:             pair,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array([-7i32, 9i32]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", pair.low, pair.high);
// CHECK-NEXT:     let packed: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<pair, std::simd::Simd<i32, 2>>(pair)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", packed[0], packed[1]);
// CHECK-NEXT:     let mut words: __SlateAlign16<[u32; 4]> = __SlateAlign16([0u32; 4]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(floats) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((words.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<[u32; 4]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut rebuilt: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(floats)
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let __slate_printf_arg0 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(rebuilt) as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             (words.0.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             (std::mem::size_of::<[u32; 4]>() as u64) as usize,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
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
