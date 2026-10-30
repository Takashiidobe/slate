#include <stdio.h>

#define VC __attribute__((vectorcall))

static int VC add_ints(int a, long long b, unsigned char c, short d, int e) {
  return a + (int)b + c + d + e;
}

static double VC mix(int a, double b, float c, long long d, double e, float f,
                     double g, int h, double i) {
  return a + b + c + d + e + f + g + h + i;
}

static float VC scale(const float *p, float k) { return *p * k; }

static void VC store(int *out, double v) { *out = (int)(v * 2.0); }

typedef double(VC *mix_fn)(int, double, float, long long, double, float,
                           double, int, double);

int main(void) {
  float x = 1.5f;
  int out = 0;
  mix_fn fp = mix;
  printf("%d\n", add_ints(1, 2, 3, 4, 5));
  printf("%.2f\n", mix(1, 2.5, 3.25f, 4, 5.5, 6.75f, 7.0, 8, 9.125));
  printf("%.2f\n", fp(-1, 0.5, 0.25f, -4, 1.5, 2.0f, 3.0, -8, 0.125));
  printf("%.2f\n", (double)scale(&x, 4.0f));
  store(&out, 10.25);
  printf("%d\n", out);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(abi_vectorcall)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "vectorcall-unwind" fn add_ints(a: i32, b: i64, c: u8, d: i16, e: i32) -> i32 {
// CHECK-NEXT:     a + (b as i32) + ((c as u32) as i32) + (d as i32) + e
// CHECK-NEXT: }
// CHECK-NEXT: extern "vectorcall-unwind" fn mix(
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: f64,
// CHECK-NEXT:     c: f32,
// CHECK-NEXT:     d: i64,
// CHECK-NEXT:     e: f64,
// CHECK-NEXT:     f: f32,
// CHECK-NEXT:     g: f64,
// CHECK-NEXT:     h: i32,
// CHECK-NEXT:     i: f64,
// CHECK-NEXT: ) -> f64 {
// CHECK-NEXT:     (a as f64) + b + (c as f64) + (d as f64) + e + (f as f64) + g + (h as f64) + i
// CHECK-NEXT: }
// CHECK-NEXT: extern "vectorcall-unwind" fn scale(p: *const f32, k: f32) -> f32 {
// CHECK-NEXT:     (unsafe { *p }) * k
// CHECK-NEXT: }
// CHECK-NEXT: extern "vectorcall-unwind" fn store(out: *mut i32, v: f64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out = (v * 2.0f64) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: f32 = 1.5f32;
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     let fp: Option<
// CHECK-NEXT:         unsafe extern "vectorcall-unwind" fn(
// CHECK-NEXT:             i32,
// CHECK-NEXT:             f64,
// CHECK-NEXT:             f32,
// CHECK-NEXT:             i64,
// CHECK-NEXT:             f64,
// CHECK-NEXT:             f32,
// CHECK-NEXT:             f64,
// CHECK-NEXT:             i32,
// CHECK-NEXT:             f64,
// CHECK-NEXT:         ) -> f64,
// CHECK-NEXT:     > = Some(mix);
// CHECK-NEXT:     println!("{}", add_ints(1i32, 2i64, 3u8, 4i16, 5i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f\n".as_ptr(),
// CHECK-NEXT:             mix(1i32, 2.5f64, 3.25f32, 4i64, 5.5f64, 6.75f32, 7.0f64, 8i32, 9.125f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f\n".as_ptr(),
// CHECK-NEXT:             fp
// CHECK-NEXT:                 .unwrap()(
// CHECK-NEXT:                 -1i32,
// CHECK-NEXT:                 0.5f64,
// CHECK-NEXT:                 0.25f32,
// CHECK-NEXT:                 -4i64,
// CHECK-NEXT:                 1.5f64,
// CHECK-NEXT:                 2.0f32,
// CHECK-NEXT:                 3.0f64,
// CHECK-NEXT:                 -8i32,
// CHECK-NEXT:                 0.125f64,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f\n".as_ptr(),
// CHECK-NEXT:             scale(std::ptr::addr_of_mut!(x) as *const f32, 4.0f32) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     store(std::ptr::addr_of_mut!(out), 10.25f64);
// CHECK-NEXT:     println!("{}", out);
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
