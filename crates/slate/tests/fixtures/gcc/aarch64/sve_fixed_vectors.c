// { dg-options "-march=armv8.2-a+sve -msve-vector-bits=512" }
#include <arm_sve.h>
#include <stdio.h>

typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(512)));
typedef svfloat64_t fixed_f64 __attribute__((arm_sve_vector_bits(512)));
typedef svuint8_t fixed_u8 __attribute__((arm_sve_vector_bits(512)));
typedef svbool_t fixed_bool __attribute__((arm_sve_vector_bits(512)));
typedef int gnu_i32 __attribute__((vector_size(64)));
typedef unsigned char gnu_u8x8 __attribute__((vector_size(8)));

struct holder {
    fixed_i32 vector;
    fixed_bool predicate;
    int tail;
};

static fixed_i32 counters = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

static fixed_i32 scale(fixed_i32 a, int factor) { return a * factor + (a >> 1); }

static fixed_f64 halve(fixed_f64 a) { return a / 2.0; }

static gnu_i32 as_gnu(fixed_i32 a) { return a; }

static fixed_bool predicate_of(gnu_u8x8 bytes) { return (fixed_bool)bytes; }

int main(void) {
    struct holder h = {counters, predicate_of((gnu_u8x8){0xff, 0x0f, 0xf0, 0x55, 0xaa, 0x01, 0x80, 0x3c}), 42};
    fixed_i32 scaled = scale(h.vector, 3);
    gnu_i32 plain = as_gnu(scaled);
    gnu_i32 mask = scaled > counters * 3;
    fixed_f64 doubles = {1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0};
    fixed_f64 halves = halve(doubles);
    fixed_u8 bytes = (fixed_u8)scaled;
    fixed_bool high = predicate_of((gnu_u8x8){0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0});
    fixed_bool predicates = (h.predicate & high) | ~h.predicate;
    long sum = 0;
    for (int i = 0; i < 16; i++) {
        sum += scaled[i] * (i + 1) + plain[i] - mask[i];
    }
    for (int i = 0; i < 8; i++) {
        printf("%g %d ", halves[i], predicates[i]);
    }
    printf("\n%ld %u %u %zu %zu %zu %d\n", sum, bytes[0], bytes[5], sizeof(fixed_i32),
           sizeof(fixed_bool), sizeof(struct holder), h.tail);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-march=armv8.2-a+sve -msve-vector-bits=512" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holder {
// CHECK-NEXT:     vector: core::simd::Simd<i32, 16>,
// CHECK-NEXT:     predicate: core::simd::Simd<i8, 8>,
// CHECK-NEXT:     tail: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut counters: core::simd::Simd<i32, 16> = core::simd::Simd::from_array([
// CHECK-NEXT:     1i32, 2i32, 3i32, 4i32, 5i32, 6i32, 7i32, 8i32, 9i32, 10i32, 11i32, 12i32, 13i32, 14i32, 15i32,
// CHECK-NEXT:     16i32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn scale(a: core::simd::Simd<i32, 16>, factor: i32) -> core::simd::Simd<i32, 16> {
// CHECK-NEXT:     return a * core::simd::Simd::splat(factor) + (a >> core::simd::Simd::splat(1i32));
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn halve(a: core::simd::Simd<f64, 8>) -> core::simd::Simd<f64, 8> {
// CHECK-NEXT:     return a / core::simd::Simd::splat(2.0f64);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn as_gnu(a: core::simd::Simd<i32, 16>) -> core::simd::Simd<i32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 16>, core::simd::Simd<i32, 16>>(a)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn predicate_of(bytes: core::simd::Simd<u8, 8>) -> core::simd::Simd<i8, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u8, 8>, core::simd::Simd<i8, 8>>(bytes)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<u8, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: core::simd::Simd<u8, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let h: holder = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: holder = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].vector = unsafe { counters };
// CHECK-NEXT:         [[T_0]].predicate = unsafe {
// CHECK-NEXT:             predicate_of(*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([
// CHECK-NEXT:                     255u8, 15u8, 240u8, 85u8, 170u8, 1u8, 128u8, 60u8,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]].tail = 42i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let scaled: core::simd::Simd<i32, 16> = unsafe { scale(h.vector, 3i32) };
// CHECK-NEXT:     let plain: core::simd::Simd<i32, 16> = unsafe { as_gnu(scaled) };
// CHECK-NEXT:     let mask: core::simd::Simd<i32, 16> = {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: core::simd::Simd<i32, 16> = scaled;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = [[T_1]].to_array();
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: core::simd::Simd<i32, 16> = (unsafe { counters }) * core::simd::Simd::splat(3i32);
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = [[T_3]].to_array();
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: core::simd::Simd<i32, 16> =
// CHECK-NEXT:             core::simd::Simd::from_array(core::array::from_fn(|__lane| {
// CHECK-NEXT:                 -(([[T_2]][__lane] > [[T_4]][__lane]) as i32)
// CHECK-NEXT:             }));
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let doubles: core::simd::Simd<f64, 8> = core::simd::Simd::from_array([
// CHECK-NEXT:         1.0f64, 3.0f64, 5.0f64, 7.0f64, 9.0f64, 11.0f64, 13.0f64, 15.0f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let halves: core::simd::Simd<f64, 8> = unsafe { halve(doubles) };
// CHECK-NEXT:     let bytes: core::simd::Simd<u8, 64> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 16>, core::simd::Simd<u8, 64>>(scaled)
// CHECK-NEXT:     };
// CHECK-NEXT:     let high: core::simd::Simd<i8, 8> = unsafe {
// CHECK-NEXT:         predicate_of(*{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = core::simd::Simd::from_array([
// CHECK-NEXT:                 240u8, 240u8, 240u8, 240u8, 240u8, 240u8, 240u8, 240u8,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let predicates: core::simd::Simd<i8, 8> = h.predicate & high | !h.predicate;
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         sum += (scaled[i as usize] * (i + 1) + plain[i as usize] - mask[i as usize]) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%g %d ".as_ptr(),
// CHECK-NEXT:                 halves[i as usize],
// CHECK-NEXT:                 predicates[i as usize] as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"\n%ld %u %u %zu %zu %zu %d\n".as_ptr(),
// CHECK-NEXT:             sum,
// CHECK-NEXT:             bytes[0] as i32,
// CHECK-NEXT:             bytes[5] as i32,
// CHECK-NEXT:             64u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             80u64,
// CHECK-NEXT:             h.tail,
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
