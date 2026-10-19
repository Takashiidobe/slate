#include <stdio.h>

struct record {
  int  head[3];
  char tag;
  char tail[2];
};

static int self(int value) { return value + 1; }
static int crate(void) { return 7; }

static int shifted(int x) {
  if (x > 32)
    return (x << -64) & 255;
  return x;
}

static int divided(int x) {
  int zero = 0;
  if (x > 100)
    return x / zero;
  return x % 7;
}

static int same(void) { return 3; }

int main(void) {
  int a = 0, b = 2, c = -1;
  int elvis = a || b < 0 || b > 1 ?: c;
  int other = a || b < 0 ?: c;
  unsigned long tail = (unsigned long)&((struct record *)0)->tail;
  unsigned long tag  = (unsigned long)&((struct record *)0)->tag;
  printf("%d %d\n", self(4), crate());
  printf("%d %d %d\n", shifted(5), divided(20), same == same);
  printf("%d %d %lu %lu\n", elvis, other, tail, tag);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct record {
// CHECK-NEXT:     head: [i32; 3],
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     tail: [i8; 2],
// CHECK-NEXT: }
// CHECK-NEXT: fn self_(value: i32) -> i32 {
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn crate_() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn shifted(x: i32) -> i32 {
// CHECK-NEXT:     if x > 32 {
// CHECK-NEXT:         return x << -64i32 & 255;
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn divided(x: i32) -> i32 {
// CHECK-NEXT:     let zero: i32 = 0;
// CHECK-NEXT:     if x > 100 {
// CHECK-NEXT:         return x / zero;
// CHECK-NEXT:     }
// CHECK-NEXT:     x % 7
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.same")]
// CHECK-NEXT: extern "C-unwind" fn same() -> i32 {
// CHECK-NEXT:     3
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = 0;
// CHECK-NEXT:     let b: i32 = 2;
// CHECK-NEXT:     let c: i32 = -1;
// CHECK-NEXT:     let mut elvis: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (a != 0 || b < 0 || b > 1) as i32;
// CHECK-NEXT:     elvis = if [[V_0]] != 0 { [[V_0]] } else { c };
// CHECK-NEXT:     let mut other: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = (a != 0 || b < 0) as i32;
// CHECK-NEXT:     other = if [[V_1]] != 0 { [[V_1]] } else { c };
// CHECK-NEXT:     let tail: u64 = (13usize as *mut [i8; 2]) as u64;
// CHECK-NEXT:     let tag: u64 = (12usize as *mut i8) as u64;
// CHECK-NEXT:     println!("{} {}", self_(4i32), crate_());
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", shifted(5i32), divided(20i32), (({ let __slate_function : Option <
// CHECK-NEXT:         unsafe extern "C-unwind" fn () -> i32 > = Some(same); __slate_function }) ==
// CHECK-NEXT:         Some(same)) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { printf(c"%d %d %lu %lu\n".as_ptr(), elvis, other, tail, tag) };
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
