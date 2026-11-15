// { dg-options "-O2" }
#include <stdio.h>

void __compiletime_assert_1(void);

#define is_nonneg(x) (__builtin_constant_p(x) && (x) >= 0)

static inline __attribute__((always_inline)) unsigned min_segments(unsigned a, int b) {
  int y = b;
  if (!is_nonneg(y))
    __compiletime_assert_1();
  return a < (unsigned)y ? a : (unsigned)y;
}

unsigned limit(unsigned segments) { return min_segments(segments, 2); }

enum freq { NORMAL, LIKELY };

static inline __attribute__((always_inline)) int pick(const enum freq fr, int x) {
  if (!__builtin_constant_p(fr))
    __compiletime_assert_1();
  return fr == LIKELY ? x : -x;
}

int likely_pick(int x) { return pick(LIKELY, x); }

int query(int v, double d) { return __builtin_constant_p(v) + __builtin_constant_p(d); }

int main(void) {
  printf("%u %u %d %d\n", limit(1), limit(9), query(3, 1.5), likely_pick(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2" }
// CHECK-NEXT: macro_rules! is_nonneg {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         (unsafe { __slate_intrinsic_a39e427d3b6c2df4($x) }) && ($x) >= 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __compiletime_assert_1();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.is.constant.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_1c5364fcd9da51fe([[ARG_0:arg[0-9]+]]: u32) -> bool;
// CHECK-NEXT:     #[link_name = "llvm.is.constant.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_a39e427d3b6c2df4([[ARG_0]]: i32) -> bool;
// CHECK-NEXT:     #[link_name = "llvm.is.constant.f64"]
// CHECK-NEXT:     fn __slate_intrinsic_e1dafd5b681c8e0d([[ARG_0]]: f64) -> bool;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn min_segments(a: u32, y: i32) -> u32 {
// CHECK-NEXT:     if !is_nonneg!(y) {
// CHECK-NEXT:         unsafe { __compiletime_assert_1() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if a < (y as u32) { a } else { y as u32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn limit(segments: u32) -> u32 {
// CHECK-NEXT:     min_segments(segments, 2i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pick(fr: u32, x: i32) -> i32 {
// CHECK-NEXT:     if !unsafe { __slate_intrinsic_1c5364fcd9da51fe(fr as u32) } {
// CHECK-NEXT:         unsafe { __compiletime_assert_1() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (fr as u32) == 1 { x } else { -x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn likely_pick(x: i32) -> i32 {
// CHECK-NEXT:     pick(1u32, x)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn query(v: i32, d: f64) -> i32 {
// CHECK-NEXT:     ((unsafe { __slate_intrinsic_a39e427d3b6c2df4(v) }) as i32)
// CHECK-NEXT:         + ((unsafe { __slate_intrinsic_e1dafd5b681c8e0d(d) }) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %d %d\n".as_ptr(),
// CHECK-NEXT:             limit(1u32),
// CHECK-NEXT:             limit(9u32),
// CHECK-NEXT:             query(3i32, 1.5f64),
// CHECK-NEXT:             likely_pick(4i32),
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
