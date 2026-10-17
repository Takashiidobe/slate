#include <stdio.h>

enum Op { Add, Sub, Mul, Halt };

static int calls = 0;

static int next_value(int value) {
  calls++;
  return value;
}

static int stacked(int x) {
  switch (x) {
  case 1:
  case 2:
  case 3:
    return 10;
  default:
  case 9:
    return 90;
  case 4: {
    int doubled = x * 2;
    return doubled;
  }
  }
}

static int no_default(char c) {
  int out = 0;
  switch (c) {
  case 'a':
    out = 1;
    break;
  case 'z':
    out = 26;
    break;
  }
  return out;
}

static int nested(int outer, int inner) {
  int out = 0;
  switch (outer) {
  case 0:
    switch (inner) {
    case 0:
      out = 100;
      break;
    default:
      out = 101;
      break;
    }
    out += 1;
    break;
  default:
    out = -1;
    break;
  }
  return out;
}

static int run(const enum Op *ops) {
  int acc = 1;
  int i = 0;
  while (1) {
    enum Op op = ops[i];
    i++;
    switch (op) {
    case Add:
      acc += 3;
      break;
    case Sub:
      acc -= 1;
      continue;
    case Mul: {
      acc *= 2;
      break;
    }
    case Halt:
      return acc;
    }
    acc += 100;
  }
}

static int first_over(const int *values, int count, int limit) {
  int found = -1;
  for (int i = 0; i < count; i++) {
    if (values[i] > limit) {
      found = i;
      break;
    }
  }
  return found;
}

int main(void) {
  enum Op ops[] = {Add, Sub, Mul, Add, Halt};
  int values[] = {3, 8, 1, 12, 7};
  int picked = 0;
  switch (next_value(2)) {
  case 1:
    picked = 1;
    break;
  case 2:
    picked = 2;
    break;
  }
  printf("%d %d %d %d %d\n", stacked(2), stacked(9), stacked(4), stacked(42), stacked(0));
  printf("%d %d %d\n", no_default('a'), no_default('z'), no_default('q'));
  printf("%d %d %d\n", nested(0, 0), nested(0, 5), nested(1, 0));
  printf("%d %d %d\n", run(ops), first_over(values, 5, 10), first_over(values, 5, 50));
  printf("%d %d\n", picked, calls);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut calls: i32 = 0 as i32;
// CHECK-NEXT: fn next_value(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn stacked(x: i32) -> i32 {
// CHECK-NEXT:     match x {
// CHECK-NEXT:         1..=3 => {
// CHECK-NEXT:             return 10;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             let doubled: i32 = x * 2;
// CHECK-NEXT:             return doubled;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 90;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn no_default(c: i8) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match c as i32 {
// CHECK-NEXT:             97 => {
// CHECK-NEXT:                 out = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             122 => {
// CHECK-NEXT:                 out = 26;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn nested(outer: i32, inner: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match outer {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: {
// CHECK-NEXT:                     match inner {
// CHECK-NEXT:                         0 => {
// CHECK-NEXT:                             out = 100;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             out = 101;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 out += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 out = -1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn run(ops: *const u32) -> i32 {
// CHECK-NEXT:     let mut acc: i32 = 1;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_CONTINUE_0:__slate_continue_[0-9]+]]: {
// CHECK-NEXT:             let op: u32 = unsafe { *unsafe { ops.offset(i as isize) } };
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: {
// CHECK-NEXT:                 match op as u32 {
// CHECK-NEXT:                     0 => {
// CHECK-NEXT:                         acc += 3;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     1 => {
// CHECK-NEXT:                         acc -= 1;
// CHECK-NEXT:                         break '[[SLATE_CONTINUE_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     2 => {
// CHECK-NEXT:                         acc *= 2;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     3 => {
// CHECK-NEXT:                         return acc;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {}
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             acc += 100;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn first_over(values: *const i32, count: i32, limit: i32) -> i32 {
// CHECK-NEXT:     let mut found: i32 = -1;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         if (unsafe { *unsafe { values.offset(i as isize) } }) > limit {
// CHECK-NEXT:             found = i;
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     found
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ops: __SlateAlign16<[u32; 5]> = __SlateAlign16([
// CHECK-NEXT:         0 as u32,
// CHECK-NEXT:         1 as u32,
// CHECK-NEXT:         2 as u32,
// CHECK-NEXT:         0 as u32,
// CHECK-NEXT:         3 as u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         12 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut picked: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match next_value(2 as i32) {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 picked = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 picked = 2;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", stacked(2 as i32), stacked(9 as i32), stacked(4 as i32),
// CHECK-NEXT:         stacked(42 as i32), stacked(0 as i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", no_default(97 as i8), no_default(122 as i8), no_default(113 as i8)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", nested(0 as i32, 0 as i32), nested(0 as i32, 5 as i32), nested(1 as
// CHECK-NEXT:         i32, 0 as i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg0 = run(ops.0.as_mut_ptr() as *const u32);
// CHECK-NEXT:     let __slate_printf_arg1 = first_over(
// CHECK-NEXT:         values.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg2 = first_over(
// CHECK-NEXT:         values.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         50 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", __slate_printf_arg0, __slate_printf_arg1, __slate_printf_arg2);
// CHECK-NEXT:     println!("{} {}", picked, unsafe { calls });
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
