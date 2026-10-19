#include <stdio.h>

typedef signed char v16qi __attribute__((vector_size(16)));
typedef unsigned char v16uqi __attribute__((vector_size(16)));
typedef int v4si __attribute__((vector_size(16)));
typedef unsigned v4su __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));
typedef float v4sf __attribute__((vector_size(16)));
typedef double v2df __attribute__((vector_size(16)));

int main(void) {
  v4si i = {3, -7, 12, 5};
  v4su u = {3, 7, 12, 5};
  v16uqi b = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 250};
  v2di l = {-4, 9};
  v4sf f = {1.5f, -2.5f, 4.0f, 0.5f};
  v2df d = {2.5, -8.0};

  printf("%d %d %d\n", __builtin_reduce_add(i), __builtin_reduce_mul(i), __builtin_reduce_max(i));
  printf("%d %u %u\n", __builtin_reduce_min(i), __builtin_reduce_and(u), __builtin_reduce_or(u));
  printf("%u %u\n", __builtin_reduce_xor(u), __builtin_reduce_add(b));
  printf("%lld %lld\n", __builtin_reduce_add(l), __builtin_reduce_min(l));
  printf("%.2f %.2f\n", __builtin_reduce_max(f), __builtin_reduce_min(d));
  printf("%.2f %.2f\n", __builtin_reduce_maximum(f), __builtin_reduce_minimum(d));

  v4si ia = __builtin_elementwise_abs(i);
  v4si imax = __builtin_elementwise_max(i, ia);
  v4si imin = __builtin_elementwise_min(i, ia);
  v4sf fa = __builtin_elementwise_abs(f);
  v2df dmax = __builtin_elementwise_max(d, (v2df){0.0, 1.0});
  printf("%d %d %d %d\n", ia[0], ia[1], ia[2], ia[3]);
  printf("%d %d %d %d\n", imax[0], imax[1], imax[2], imax[3]);
  printf("%d %d %d %d\n", imin[0], imin[1], imin[2], imin[3]);
  printf("%.2f %.2f %.2f %.2f\n", fa[0], fa[1], fa[2], fa[3]);
  printf("%.2f %.2f\n", dmax[0], dmax[1]);
  printf("%d %d %.2f\n", __builtin_elementwise_abs(-5), __builtin_elementwise_max(3, 9),
         __builtin_elementwise_min(2.5, -1.0));
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
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_1d26b5d36a7eb498([[ARG_0:arg[0-9]+]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.fmax.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_26093aef5960597([[ARG_0]]: std::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.xor.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_2784696dc045b6b0([[ARG_0]]: std::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.smin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_37cf22be76d4abf7(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.minnum.f64"]
// CHECK-NEXT:     fn __slate_intrinsic_4013add02cebaf25([[ARG_0]]: f64, [[ARG_1]]: f64) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_4e03cac4e2787ee5([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.and.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_5726e58fbf88b603([[ARG_0]]: std::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smin.v2i64"]
// CHECK-NEXT:     fn __slate_intrinsic_8b4a63d8549d5fe9([[ARG_0]]: std::simd::Simd<i64, 2>) -> i64;
// CHECK-NEXT:     #[link_name = "llvm.maxnum.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_94c3816d21b3babc(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.or.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_aa0abfe0bf34f9a3([[ARG_0]]: std::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v2i64"]
// CHECK-NEXT:     fn __slate_intrinsic_b914587e1d9172a0([[ARG_0]]: std::simd::Simd<i64, 2>) -> i64;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.fminimum.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_bd507a298a8afc73([[ARG_0]]: std::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.fmaximum.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_c218738e4a75ae17([[ARG_0]]: std::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.mul.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e1d90ef8d282d235([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.smax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e3673729f976a086(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e38776902fc0d037([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.fmin.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_eb3fade2525a27fc([[ARG_0]]: std::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.smax.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_ebb2f05c022cc220([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_f05269fefaa055b0([[ARG_0]]: std::simd::Simd<u8, 16>) -> u8;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         3i32,
// CHECK-NEXT:         -7i32,
// CHECK-NEXT:         12i32,
// CHECK-NEXT:         5i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let u: std::simd::Simd<u32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         3u32, 7u32, 12u32, 5u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let b: std::simd::Simd<u8, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         1u8, 2u8, 3u8, 4u8, 5u8, 6u8, 7u8, 8u8, 9u8, 10u8, 11u8, 12u8, 13u8, 14u8, 15u8,
// CHECK-NEXT:         250u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let l: std::simd::Simd<i64, 2> = std::simd::Simd::from_array([-4i64, 9i64]);
// CHECK-NEXT:     let mut f: std::simd::Simd<f32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         1.5f32,
// CHECK-NEXT:         -2.5f32,
// CHECK-NEXT:         4.0f32,
// CHECK-NEXT:         0.5f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let d: std::simd::Simd<f64, 2> = std::simd::Simd::from_array([2.5f64, -8.0f64]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe { __slate_intrinsic_1d26b5d36a7eb498(i) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_e1d90ef8d282d235(i) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_4e03cac4e2787ee5(i) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %u %u\n".as_ptr(),
// CHECK-NEXT:             __slate_intrinsic_e38776902fc0d037(i),
// CHECK-NEXT:             __slate_intrinsic_5726e58fbf88b603(u),
// CHECK-NEXT:             __slate_intrinsic_aa0abfe0bf34f9a3(u),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             __slate_intrinsic_2784696dc045b6b0(u),
// CHECK-NEXT:             (__slate_intrinsic_f05269fefaa055b0(b) as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld %lld\n".as_ptr(),
// CHECK-NEXT:             __slate_intrinsic_b914587e1d9172a0(l),
// CHECK-NEXT:             __slate_intrinsic_8b4a63d8549d5fe9(l),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f %.2f\n".as_ptr(),
// CHECK-NEXT:             __slate_intrinsic_26093aef5960597(f) as f64,
// CHECK-NEXT:             __slate_intrinsic_eb3fade2525a27fc(d),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f %.2f\n".as_ptr(),
// CHECK-NEXT:             __slate_intrinsic_c218738e4a75ae17(f) as f64,
// CHECK-NEXT:             __slate_intrinsic_bd507a298a8afc73(d),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let ia: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:         i.to_array().map(|__lane| __lane.wrapping_abs()),
// CHECK-NEXT:     );
// CHECK-NEXT:     let imax: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         __slate_intrinsic_e3673729f976a086(i, ia)
// CHECK-NEXT:     };
// CHECK-NEXT:     let imin: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         __slate_intrinsic_37cf22be76d4abf7(i, ia)
// CHECK-NEXT:     };
// CHECK-NEXT:     let fa: std::simd::Simd<f32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:         f.to_array().map(|__lane| __lane.abs()),
// CHECK-NEXT:     );
// CHECK-NEXT:     let dmax: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         __slate_intrinsic_94c3816d21b3babc(
// CHECK-NEXT:             d,
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([0.0f64, 1.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {} {}", ia[0], ia[1], ia[2], ia[3]);
// CHECK-NEXT:     println!("{} {} {} {}", imax[0], imax[1], imax[2], imax[3]);
// CHECK-NEXT:     println!("{} {} {} {}", imin[0], imin[1], imin[2], imin[3]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f %.2f %.2f %.2f\n".as_ptr(),
// CHECK-NEXT:             fa[0] as f64,
// CHECK-NEXT:             fa[1] as f64,
// CHECK-NEXT:             fa[2] as f64,
// CHECK-NEXT:             fa[3] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.2f %.2f\n".as_ptr(), dmax[0], dmax[1]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %.2f\n".as_ptr(),
// CHECK-NEXT:             (-5i32).wrapping_abs(),
// CHECK-NEXT:             __slate_intrinsic_ebb2f05c022cc220(3i32, 9i32),
// CHECK-NEXT:             __slate_intrinsic_4013add02cebaf25(2.5f64, -1.0f64),
// CHECK-NEXT:         )
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
