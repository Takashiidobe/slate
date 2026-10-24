#include <stdio.h>

static int scan(int n, int mode) {
  int i = 0, s = 0;
  if (mode) {
    for (;;) {
      if (i >= n) goto done;
      s += i;
      i++;
    }
  }
  s += 100;
done:
  printf("scan %d %d\n", s, i);
  return s;
}

static int flags(int c) {
  int fl = 0, w = 0, done = 0;
  do {
    switch (c) {
    case '-': fl = 1; c = '+'; break;
    case '+': fl = 2; c = 'x'; break;
    default: done = 1; break;
    case 'l':
      fl = 3;
      c += 1;
      if (c == 'l') { c += 1; fl = 4; }
      done = 1;
      break;
    case '1': case '2': {
      unsigned wx = c - '0';
      while ((c = c + 1) > '0' && c < '9') wx = wx * 10 + c - '0';
      w = wx;
      if (c != '.' && c != 'l') done = 1; else c--;
      break;
    }
    }
  } while (!done);
  printf("flags %d %d %d\n", fl, w, c);
  return fl + w;
}

int main(void) {
  scan(3, 0);
  scan(3, 1);
  flags('-');
  flags('l');
  flags('1');
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn scan(n: i32, mode: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         s = 0;
// CHECK-NEXT:         if mode != 0 {
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if i >= n {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     s += i;
// CHECK-NEXT:                     i += 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             s += 100;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("scan {} {}", s, i);
// CHECK-NEXT:         return s;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn flags(mut c: i32) -> i32 {
// CHECK-NEXT:     let mut fl: i32 = 0;
// CHECK-NEXT:     let mut w: i32 = 0;
// CHECK-NEXT:     let mut done: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         match c {
// CHECK-NEXT:             45 => {
// CHECK-NEXT:                 fl = 1;
// CHECK-NEXT:                 c = 43;
// CHECK-NEXT:             }
// CHECK-NEXT:             43 => {
// CHECK-NEXT:                 fl = 2;
// CHECK-NEXT:                 c = 120;
// CHECK-NEXT:             }
// CHECK-NEXT:             108 => {
// CHECK-NEXT:                 fl = 3;
// CHECK-NEXT:                 c += 1;
// CHECK-NEXT:                 if c == 108 {
// CHECK-NEXT:                     c += 1;
// CHECK-NEXT:                     fl = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 done = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             49 | 50 => {
// CHECK-NEXT:                 let mut wx: u32 = (c - 48) as u32;
// CHECK-NEXT:                 loop {
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: i32 = c + 1;
// CHECK-NEXT:                     c = [[V_0]];
// CHECK-NEXT:                     if !([[V_0]] > 48 && c < 57) {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     wx = wx.wrapping_mul(10).wrapping_add(c as u32).wrapping_sub(48);
// CHECK-NEXT:                 }
// CHECK-NEXT:                 w = wx as i32;
// CHECK-NEXT:                 if c != 46 && c != 108 {
// CHECK-NEXT:                     done = 1;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     c -= 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 done = 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if done != 0 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("flags {} {} {}", fl, w, c);
// CHECK-NEXT:     fl + w
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     scan(3i32, 0i32);
// CHECK-NEXT:     scan(3i32, 1i32);
// CHECK-NEXT:     flags(45i32);
// CHECK-NEXT:     flags(108i32);
// CHECK-NEXT:     flags(49i32);
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
