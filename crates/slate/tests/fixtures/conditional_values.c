#include <stdio.h>

struct Pair {
  int a;
  int b;
};

static int sign(int x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

static int clamp_add(int x, int y) { return (x > y ? x : y) * 2 + (x < y ? x : y); }

static const char *pick(int which, const char *left, const char *right) {
  return which ? left : right;
}

static int sum_pair(struct Pair pair) { return pair.a * 10 + pair.b; }

int main(void) {
  int values[] = {5, -7, 0, 12};
  int total = 0;
  for (int i = 0; i < 4; i++)
    total += sign(values[i]) * 100 + (values[i] % 2 ? 7 : 3);
  struct Pair first = {1, 2};
  struct Pair second = {3, 4};
  int x = 4, y = 9;
  int seq = x + y;
  int nested = (x > 3 ? (y < 5 ? 1 : 2) : 3) + (y > 8);
  double half = x > y ? x / 2.0 : y / 2.0;
  unsigned long wide = y > 0 ? 4000000000UL : 1UL;
  int buffer[3] = {10, 20, 30};
  int *cursor = x ? &buffer[2] : &buffer[0];
  if (x > 2 ? y > 2 : y < 2)
    total += 1000;
  printf("%d %d %d %d\n", total, clamp_add(3, 8), clamp_add(9, -1), seq);
  printf("%d %.1f %lu %d\n", nested, half, wide, *cursor);
  printf("%s %s\n", pick(1, "left", "right"), pick(0, "left", "right"));
  printf("%d\n", sum_pair(y > x ? first : second));
  return sign(-x) + 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
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
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sign(x: i32) -> i32 {
// CHECK-NEXT:     if x > 0 { 1 } else { if x < 0 { -1 } else { 0 } }
// CHECK-NEXT: }
// CHECK-NEXT: fn clamp_add(x: i32, y: i32) -> i32 {
// CHECK-NEXT:     (if x > y { x } else { y }) * 2 + if x < y { x } else { y }
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(which: i32, left: *const i8, right: *const i8) -> *const i8 {
// CHECK-NEXT:     if which != 0 { left } else { right }
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_pair(pair: Pair) -> i32 {
// CHECK-NEXT:     pair.a * 10 + pair.b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: __SlateAlign16<[i32; 4]> = __SlateAlign16([5i32, -7i32, 0i32, 12i32]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = sign(values.0[i as usize]) * 100
// CHECK-NEXT:             + if values.0[i as usize] % 2 != 0 { 7 } else { 3 };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let first: Pair = Pair { a: 1, b: 2 };
// CHECK-NEXT:     let second: Pair = Pair { a: 3, b: 4 };
// CHECK-NEXT:     let x: i32 = 4;
// CHECK-NEXT:     let y: i32 = 9;
// CHECK-NEXT:     let seq: i32 = x + y;
// CHECK-NEXT:     let nested: i32 = (if x > 3 { if y < 5 { 1 } else { 2 } } else { 3 })
// CHECK-NEXT:         + ((y > 8) as i32);
// CHECK-NEXT:     let half: f64 = if x > y { (x as f64) / 2.0f64 } else { (y as f64) / 2.0f64 };
// CHECK-NEXT:     let wide: u64 = if y > 0 { 4000000000 } else { 1 };
// CHECK-NEXT:     let mut buffer: [i32; 3] = [10, 20, 30];
// CHECK-NEXT:     let cursor: *mut i32 = if x != 0 {
// CHECK-NEXT:         unsafe { (buffer.as_mut_ptr() as *mut i32).add(2) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { (buffer.as_mut_ptr() as *mut i32).add(0) }
// CHECK-NEXT:     };
// CHECK-NEXT:     if (if x > 2 { (y > 2) as i32 } else { (y < 2) as i32 }) != 0 {
// CHECK-NEXT:         total += 1000;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {} {} {}", total, clamp_add(3i32, 8i32), clamp_add(9i32, - 1i32), seq);
// CHECK-NEXT:     unsafe { printf(c"%d %.1f %lu %d\n".as_ptr(), nested, half, wide, *cursor) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s\n".as_ptr(),
// CHECK-NEXT:             pick(1i32, c"left".as_ptr(), c"right".as_ptr()),
// CHECK-NEXT:             pick(0i32, c"left".as_ptr(), c"right".as_ptr()),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", sum_pair(if y > x { first } else { second }));
// CHECK-NEXT:     sign(-x) + 1
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
