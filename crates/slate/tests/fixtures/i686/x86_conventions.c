#include <stdio.h>

struct pair {
  int a, b;
};

struct triple {
  int a, b, c;
};

static int __attribute__((stdcall)) std_sum(int a, short b, long long c, double d) {
  return a + b + (int)c + (int)d;
}

static struct triple __attribute__((stdcall)) std_make(struct pair p, int c) {
  struct triple t = {p.a, p.b, c};
  return t;
}

static long long __attribute__((thiscall)) this_mix(int *self, long long k, float f) {
  return *self * k + (long long)f;
}

static struct pair __attribute__((thiscall)) this_swap(struct pair p) {
  struct pair q = {p.b, p.a};
  return q;
}

static int __attribute__((fastcall)) fast_ints(int a, unsigned char b, int c, short d) {
  return a * 1000 + b * 100 + c * 10 + d;
}

static double __attribute__((fastcall)) fast_mix(double x, int a, float y, int b, int c) {
  return x + a * 2 + y + b * 3 + c * 4;
}

static long long __attribute__((fastcall)) fast_wide(int a, long long b) {
  return a + b;
}

typedef int(__attribute__((stdcall)) * std_fn)(int, short, long long, double);
typedef int(__attribute__((fastcall)) * fast_fn)(int, unsigned char, int, short);
typedef struct pair(__attribute__((thiscall)) * swap_fn)(struct pair);

int main(void) {
  struct pair p = {3, 4};
  int self = 7;
  std_fn sf = std_sum;
  fast_fn ff = fast_ints;
  swap_fn wf = this_swap;
  struct triple t = std_make(p, 5);
  struct pair q = wf(p);
  printf("%d %d\n", std_sum(1, 2, 3, 4.5), sf(-1, -2, -3, -4.5));
  printf("%d %d %d\n", t.a, t.b, t.c);
  printf("%lld\n", this_mix(&self, 6, 2.5f));
  printf("%d %d\n", q.a, q.b);
  printf("%d %d\n", fast_ints(1, 2, 3, 4), ff(5, 6, 7, 8));
  printf("%.2f\n", fast_mix(0.5, 1, 1.25f, 2, 3));
  printf("%lld\n", fast_wide(1, 1LL << 40));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct triple {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.std_sum")]
// CHECK-NEXT: extern "stdcall-unwind" fn std_sum(a: i32, b: i16, c: i64, d: f64) -> i32 {
// CHECK-NEXT:     a + (b as i32) + (c as i32) + (d as i32)
// CHECK-NEXT: }
// CHECK-NEXT: extern "stdcall-unwind" fn std_make(p: pair, c: i32) -> triple {
// CHECK-NEXT:     let t: triple = triple { a: p.a, b: p.b, c: c };
// CHECK-NEXT:     return t;
// CHECK-NEXT: }
// CHECK-NEXT: extern "thiscall-unwind" fn this_mix(self_: *mut i32, k: i64, f: f32) -> i64 {
// CHECK-NEXT:     ((unsafe { *self_ }) as i64) * k + (f as i64)
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.this_swap")]
// CHECK-NEXT: extern "thiscall-unwind" fn this_swap(p: pair) -> pair {
// CHECK-NEXT:     let q: pair = pair { a: p.b, b: p.a };
// CHECK-NEXT:     return q;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.fast_ints")]
// CHECK-NEXT: extern "fastcall-unwind" fn fast_ints(a: i32, b: u8, c: i32, d: i16) -> i32 {
// CHECK-NEXT:     a * 1000 + ((b as u32) as i32) * 100 + c * 10 + (d as i32)
// CHECK-NEXT: }
// CHECK-NEXT: extern "fastcall-unwind" fn fast_mix(x: f64, a: i32, y: f32, b: i32, c: i32) -> f64 {
// CHECK-NEXT:     x + ((a * 2) as f64) + (y as f64) + ((b * 3) as f64) + ((c * 4) as f64)
// CHECK-NEXT: }
// CHECK-NEXT: extern "fastcall-unwind" fn fast_wide(a: i32, b: i64) -> i64 {
// CHECK-NEXT:     (a as i64) + b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: pair = pair { a: 3, b: 4 };
// CHECK-NEXT:     let mut self_: i32 = 7;
// CHECK-NEXT:     let sf: Option<unsafe extern "stdcall-unwind" fn(i32, i16, i64, f64) -> i32> = Some(
// CHECK-NEXT:         std_sum,
// CHECK-NEXT:     );
// CHECK-NEXT:     let ff: Option<unsafe extern "fastcall-unwind" fn(i32, u8, i32, i16) -> i32> = Some(
// CHECK-NEXT:         fast_ints,
// CHECK-NEXT:     );
// CHECK-NEXT:     let wf: Option<unsafe extern "thiscall-unwind" fn(pair) -> pair> = Some(this_swap);
// CHECK-NEXT:     let t: triple = std_make(p, 5i32);
// CHECK-NEXT:     let q: pair = unsafe { wf.unwrap()(p) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", std_sum(1i32, 2i16, 3i64, 4.5f64), unsafe { sf.unwrap() (- 1i32, - 2i16,
// CHECK-NEXT:         - 3i64, - 4.5f64) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", t.a, t.b, t.c);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%lld\n".as_ptr(), this_mix(std::ptr::addr_of_mut!(self_), 6i64, 2.5f32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", q.a, q.b);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", fast_ints(1i32, 2u8, 3i32, 4i16), unsafe { ff.unwrap() (5i32, 6u8, 7i32,
// CHECK-NEXT:         8i16) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { printf(c"%.2f\n".as_ptr(), fast_mix(0.5f64, 1i32, 1.25f32, 2i32, 3i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), fast_wide(1i32, 1099511627776i64)) };
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
