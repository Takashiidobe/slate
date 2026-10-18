#include <stdio.h>

typedef short v8hi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));

static int calls;

static v8hi input(void) {
  ++calls;
  return (v8hi){-1, -32768, 1000, 1, 30000, 400, 200, 3};
}

int main(void) {
  v8hi right = {-1, 2, 1000, 1, 30000, 400, 200, 3};
  v8hi product = __builtin_ia32_pmulhuw128(input(), right);
  v4si packed = (v4si)product;
  v4si sum = packed + 2147483647;
  v4si counts = {1, 0, 0, 0};
  v4si shifted = __builtin_ia32_pslld128(packed, counts);
  printf("%d %d %d %d %d %d\n", product[0], product[1], product[2], calls,
         shifted[0] == (packed[0] << 1), sum[0] < 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.psll.d"]
// CHECK-NEXT:     fn __slate_intrinsic_65fb1843bfbf1b54(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.pmulhu.w"]
// CHECK-NEXT:     fn __slate_intrinsic_69a4a419fae1d3(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i16, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<i16, 8>;
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn input() -> std::simd::Simd<i16, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 -1 as i16,
// CHECK-NEXT:                 -32768 as i16,
// CHECK-NEXT:                 1000 as i16,
// CHECK-NEXT:                 1 as i16,
// CHECK-NEXT:                 30000 as i16,
// CHECK-NEXT:                 400 as i16,
// CHECK-NEXT:                 200 as i16,
// CHECK-NEXT:                 3 as i16,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let right: std::simd::Simd<i16, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         -1 as i16,
// CHECK-NEXT:         2 as i16,
// CHECK-NEXT:         1000 as i16,
// CHECK-NEXT:         1 as i16,
// CHECK-NEXT:         30000 as i16,
// CHECK-NEXT:         400 as i16,
// CHECK-NEXT:         200 as i16,
// CHECK-NEXT:         3 as i16,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let product: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u16, 8>,
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 std::simd::Simd<u16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_69a4a419fae1d3(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u16, 8>,
// CHECK-NEXT:                             std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 std::simd::Simd<u16, 8>,
// CHECK-NEXT:                             >(input())
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u16, 8>,
// CHECK-NEXT:                             std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 std::simd::Simd<u16, 8>,
// CHECK-NEXT:                             >(right)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let packed: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i16, 8>, std::simd::Simd<i32, 4>>(product)
// CHECK-NEXT:     };
// CHECK-NEXT:     let sum: std::simd::Simd<i32, 4> = packed
// CHECK-NEXT:         + std::simd::Simd::splat(2147483647 as i32);
// CHECK-NEXT:     let counts: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let shifted: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         __slate_intrinsic_65fb1843bfbf1b54(packed, counts)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", product[0] as i32, product[1] as i32, product[2] as i32,
// CHECK-NEXT:         unsafe { calls }, (shifted[0] == packed[0] << (1 as i32)) as i32, (sum[0] < 0) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
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
