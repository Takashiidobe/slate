#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"
#include "../abi_sve_scalable.h"

static svint32_t host_double_plus_one(svint32_t v) {
  return svadd_n_s32_x(svptrue_b32(), svmul_n_s32_x(svptrue_b32(), v, 2), 1);
}

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  mul_add_fn mul_add = (mul_add_fn)dl_sym(lib, "sve_mul_add");
  first_n_fn first_n = (first_n_fn)dl_sym(lib, "sve_first_n");
  masked_scale_fn masked_scale = (masked_scale_fn)dl_sym(lib, "sve_masked_scale");
  apply_fn apply = (apply_fn)dl_sym(lib, "sve_apply");
  if (!mul_add || !first_n || !masked_scale || !apply) {
    printf("resolve failed\n");
    return 2;
  }
  svint32_t index = svindex_s32(1, 1);
  svint32_t r = mul_add(index, svdup_n_s32(3), 7);
  int lanes[64];
  svst1_s32(svptrue_b32(), lanes, r);
  printf("mul_add: %d %d %d\n", lanes[0], lanes[1], lanes[2]);
  svbool_t pg = first_n(3);
  printf("first_n: %ld active\n", (long)svcntp_b32(svptrue_b32(), pg));
  double out[8];
  svfloat64_t scaled = masked_scale(svwhilelt_b64_u32(0, 2), svdup_n_f64(1.5), 4.0);
  svst1_f64(svptrue_b64(), out, scaled);
  printf("masked_scale: %g %g %g\n", out[0], out[1], out[2]);
  long applied = apply(host_double_plus_one, svdup_n_s32(1));
  printf("apply per lane: %ld\n", applied / (long)svcntw());
  return dl_close(lib) ? 0 : 3;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![feature(stdarch_aarch64_sve)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! dl_close {
// CHECK-NEXT:     ($handle:expr) => {
// CHECK-NEXT:         (unsafe { dlclose(($handle) as *mut core::ffi::c_void) as i32 }) == 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! dl_open {
// CHECK-NEXT:     ($path:expr) => {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             dlopen(
// CHECK-NEXT:                 (($path) as *const u8) as *const core::ffi::c_char,
// CHECK-NEXT:                 2i32 as i32,
// CHECK-NEXT:             ) as *mut ()
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! dl_sym {
// CHECK-NEXT:     ($handle:expr, $name:expr) => {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             dlsym(
// CHECK-NEXT:                 ($handle) as *mut core::ffi::c_void,
// CHECK-NEXT:                 (($name) as *const u8) as *const core::ffi::c_char,
// CHECK-NEXT:             ) as *mut ()
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::dlclose;
// CHECK-NEXT: use slate_support::dlopen;
// CHECK-NEXT: use slate_support::dlsym;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.host_double_plus_one")]
// CHECK-NEXT: unsafe extern "C-unwind" fn host_double_plus_one(
// CHECK-NEXT:     v: core::arch::aarch64::svint32_t,
// CHECK-NEXT: ) -> core::arch::aarch64::svint32_t {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::arch::aarch64::svadd_n_s32_x(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:             core::arch::aarch64::svmul_n_s32_x(core::arch::aarch64::svptrue_b32(), v, 2i32),
// CHECK-NEXT:             1i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let lib: *mut () = dl_open!(c"./libdep.so".as_ptr() as *mut u8);
// CHECK-NEXT:     if lib.is_null() {
// CHECK-NEXT:         println!("open failed");
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mul_add: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// CHECK-NEXT:             core::arch::aarch64::svint32_t,
// CHECK-NEXT:             core::arch::aarch64::svint32_t,
// CHECK-NEXT:             i32,
// CHECK-NEXT:         ) -> core::arch::aarch64::svint32_t,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// CHECK-NEXT:                     core::arch::aarch64::svint32_t,
// CHECK-NEXT:                     core::arch::aarch64::svint32_t,
// CHECK-NEXT:                     i32,
// CHECK-NEXT:                 ) -> core::arch::aarch64::svint32_t,
// CHECK-NEXT:             >,
// CHECK-NEXT:         >(dl_sym!(lib, c"sve_mul_add".as_ptr() as *mut u8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let first_n: Option<unsafe extern "C-unwind" fn(u32) -> core::arch::aarch64::svbool_t> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(u32) -> core::arch::aarch64::svbool_t>,
// CHECK-NEXT:         >(dl_sym!(lib, c"sve_first_n".as_ptr() as *mut u8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let masked_scale: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// CHECK-NEXT:             core::arch::aarch64::svbool_t,
// CHECK-NEXT:             core::arch::aarch64::svfloat64_t,
// CHECK-NEXT:             f64,
// CHECK-NEXT:         ) -> core::arch::aarch64::svfloat64_t,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// CHECK-NEXT:                     core::arch::aarch64::svbool_t,
// CHECK-NEXT:                     core::arch::aarch64::svfloat64_t,
// CHECK-NEXT:                     f64,
// CHECK-NEXT:                 ) -> core::arch::aarch64::svfloat64_t,
// CHECK-NEXT:             >,
// CHECK-NEXT:         >(dl_sym!(lib, c"sve_masked_scale".as_ptr() as *mut u8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let apply: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// CHECK-NEXT:                     core::arch::aarch64::svint32_t,
// CHECK-NEXT:                 ) -> core::arch::aarch64::svint32_t,
// CHECK-NEXT:             >,
// CHECK-NEXT:             core::arch::aarch64::svint32_t,
// CHECK-NEXT:         ) -> i64,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<
// CHECK-NEXT:                 unsafe extern "C-unwind" fn(
// CHECK-NEXT:                     Option<
// CHECK-NEXT:                         unsafe extern "C-unwind" fn(
// CHECK-NEXT:                             core::arch::aarch64::svint32_t,
// CHECK-NEXT:                         )
// CHECK-NEXT:                             -> core::arch::aarch64::svint32_t,
// CHECK-NEXT:                     >,
// CHECK-NEXT:                     core::arch::aarch64::svint32_t,
// CHECK-NEXT:                 ) -> i64,
// CHECK-NEXT:             >,
// CHECK-NEXT:         >(dl_sym!(lib, c"sve_apply".as_ptr() as *mut u8))
// CHECK-NEXT:     };
// CHECK-NEXT:     if mul_add == None || first_n == None || masked_scale == None || apply == None {
// CHECK-NEXT:         println!("resolve failed");
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let index: core::arch::aarch64::svint32_t =
// CHECK-NEXT:         unsafe { core::arch::aarch64::svindex_s32(1i32, 1i32) };
// CHECK-NEXT:     let r: core::arch::aarch64::svint32_t =
// CHECK-NEXT:         unsafe { mul_add.unwrap()(index, core::arch::aarch64::svdup_n_s32(3i32), 7i32) };
// CHECK-NEXT:     let mut lanes: [i32; 64] = [0i32; 64];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::aarch64::svst1_s32(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:             lanes.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             r,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("mul_add: {} {} {}", lanes[0], lanes[1], lanes[2]);
// CHECK-NEXT:     let pg: core::arch::aarch64::svbool_t = unsafe { first_n.unwrap()(3u32) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"first_n: %ld active\n".as_ptr(),
// CHECK-NEXT:             core::arch::aarch64::svcntp_b32(core::arch::aarch64::svptrue_b32(), pg) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut out: [f64; 8] = [0 as f64; 8];
// CHECK-NEXT:     let scaled: core::arch::aarch64::svfloat64_t = unsafe {
// CHECK-NEXT:         masked_scale.unwrap()(
// CHECK-NEXT:             core::arch::aarch64::svwhilelt_b64_u32(0u32, 2u32),
// CHECK-NEXT:             core::arch::aarch64::svdup_n_f64(1.5f64),
// CHECK-NEXT:             4.0f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::aarch64::svst1_f64(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b64(),
// CHECK-NEXT:             out.as_mut_ptr() as *mut f64,
// CHECK-NEXT:             scaled,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"masked_scale: %g %g %g\n".as_ptr(), out[0], out[1], out[2]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let applied: i64 = unsafe {
// CHECK-NEXT:         apply.unwrap()(
// CHECK-NEXT:             Some(host_double_plus_one),
// CHECK-NEXT:             core::arch::aarch64::svdup_n_s32(1i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"apply per lane: %ld\n".as_ptr(),
// CHECK-NEXT:             applied / (core::arch::aarch64::svcntw() as i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     if dl_close!(lib) { 0 } else { 3 }
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
