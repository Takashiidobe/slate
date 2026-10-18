#include <stdio.h>

static int total = 100;

static void fill(int *out, int value) { *out = value; }

static void fill_pair(long *first, long *second) {
  *first = 11;
  *second = 22;
}

static int shadow_params(int value, int step) {
  int result = value;
  {
    int value = step * 10;
    result += value;
    {
      int step = value + 1;
      result += step;
    }
  }
  return result + value + step;
}

int main(void) {
  int x = 1;
  int outer_seen = 0;
  {
    int outer = x;
    int x = outer * 5;
    outer_seen = x;
    {
      int x = 40;
      outer_seen += x;
    }
    outer_seen += x;
  }
  for (int i = 0; i < 2; i++) {
    int x = i + 7;
    outer_seen += x;
  }
  {
    int sibling = 3;
    outer_seen += sibling;
  }
  {
    int sibling = 4;
    outer_seen += sibling;
  }

  int first = 2, second = first * 3, third = second + first;
  int total = third + 1;

  int type = 1, match = 2, loop = 3, ref = 4, fn = 5, impl = 6, mod = 7;
  int keywords = type + match * loop + ref * fn + impl - mod;

  int filled;
  fill(&filled, 77);
  long a, b;
  fill_pair(&a, &b);
  int chosen;
  if (x > 0) {
    chosen = 10;
  } else {
    chosen = 20;
  }

  int sum = 0;
  for (int round = 0; round < 3; round++) {
    int fresh = 1;
    fresh += round;
    sum += fresh;
  }

  int counter = 0;
  while (counter < 3) {
    int step;
    step = counter * 2;
    counter++;
    sum += step;
  }

  printf("%d %d %d %d %d\n", x, outer_seen, first, second, third);
  printf("%d %d %d\n", total, keywords, shadow_params(2, 3));
  printf("%d %ld %ld %d %d\n", filled, a, b, chosen, sum);
  return (total + x) & 0x3f;
}

int read_static_total(void) { return total; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut total: i32 = 100 as i32;
// CHECK-NEXT: fn fill(out: &mut i32, value: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(out as *mut i32) = value;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn fill_pair(first: &mut i64, second: &mut i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(first as *mut i64) = 11;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(second as *mut i64) = 22;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn shadow_params(value: i32, step: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = value;
// CHECK-NEXT:     {
// CHECK-NEXT:         let value: i32 = step * 10;
// CHECK-NEXT:         result += value;
// CHECK-NEXT:         let step: i32 = value + 1;
// CHECK-NEXT:         result += step;
// CHECK-NEXT:     }
// CHECK-NEXT:     result + value + step
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: i32 = 1;
// CHECK-NEXT:     let mut outer_seen: i32 = 0;
// CHECK-NEXT:     let outer: i32 = x;
// CHECK-NEXT:     let [[X_0:x_[0-9]+]]: i32 = outer * 5;
// CHECK-NEXT:     outer_seen = [[X_0]];
// CHECK-NEXT:     {
// CHECK-NEXT:         let x: i32 = 40;
// CHECK-NEXT:         outer_seen += x;
// CHECK-NEXT:     }
// CHECK-NEXT:     outer_seen += [[X_0]];
// CHECK-NEXT:     for i in 0 as i32..2 as i32 {
// CHECK-NEXT:         let x: i32 = i + 7;
// CHECK-NEXT:         outer_seen += x;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let sibling: i32 = 3;
// CHECK-NEXT:         outer_seen += sibling;
// CHECK-NEXT:     }
// CHECK-NEXT:     let sibling: i32 = 4;
// CHECK-NEXT:     outer_seen += sibling;
// CHECK-NEXT:     let first: i32 = 2;
// CHECK-NEXT:     let second: i32 = first * 3;
// CHECK-NEXT:     let third: i32 = second + first;
// CHECK-NEXT:     let [[TOTAL_0:total_[0-9]+]]: i32 = third + 1;
// CHECK-NEXT:     let r#type: i32 = 1;
// CHECK-NEXT:     let r#match: i32 = 2;
// CHECK-NEXT:     let r#loop: i32 = 3;
// CHECK-NEXT:     let r#ref: i32 = 4;
// CHECK-NEXT:     let r#fn: i32 = 5;
// CHECK-NEXT:     let r#impl: i32 = 6;
// CHECK-NEXT:     let r#mod: i32 = 7;
// CHECK-NEXT:     let keywords: i32 = r#type + r#match * r#loop + r#ref * r#fn + r#impl - r#mod;
// CHECK-NEXT:     let mut filled: i32 = 0;
// CHECK-NEXT:     fill(unsafe { &mut (*std::ptr::addr_of_mut!(filled)) }, 77 as i32);
// CHECK-NEXT:     let mut a: i64 = 0;
// CHECK-NEXT:     let mut b: i64 = 0;
// CHECK-NEXT:     fill_pair(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(a)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(b)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut chosen: i32 = 0;
// CHECK-NEXT:     if x > 0 {
// CHECK-NEXT:         chosen = 10;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         chosen = 20;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for round in 0 as i32..3 as i32 {
// CHECK-NEXT:         let mut fresh: i32 = 1;
// CHECK-NEXT:         fresh += round;
// CHECK-NEXT:         sum += fresh;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut counter: i32 = 0;
// CHECK-NEXT:     while counter < 3 {
// CHECK-NEXT:         let step: i32 = counter * 2;
// CHECK-NEXT:         counter += 1;
// CHECK-NEXT:         sum += step;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {} {} {} {}", x, outer_seen, first, second, third);
// CHECK-NEXT:     println!("{} {} {}", [[TOTAL_0]], keywords, shadow_params(2 as i32, 3 as i32));
// CHECK-NEXT:     unsafe { printf(c"%d %ld %ld %d %d\n".as_ptr(), filled, a, b, chosen, sum) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     [[TOTAL_0]] + x & 63
// CHECK-NEXT: }
// CHECK-NEXT: fn read_static_total() -> i32 {
// CHECK-NEXT:     unsafe { total }
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
