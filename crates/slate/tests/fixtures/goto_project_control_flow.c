#include <stdio.h>

static int calls;
static int choose(int value) {
  calls++;
  return value % 4;
}

static void accumulate(int n, int *total) {
  int i = 0;
again:
  if (i >= n)
    goto done;
  switch (choose(i++)) {
  case 0: *total += 3; break;
  case 1: *total += 5;
  case 2: *total += 7; break;
  default: *total -= 2; break;
  }
  goto again;
done:
  *total += calls;
}

static int classify(const char *s) {
again:
  switch (*s) {
  case 'a': return 1;
  case 'b': return 2;
  case 'c': return 3;
  case 'd': return 4;
  case ' ': s++; goto again;
  case '\t': s++; goto again;
  case 'n': return 5;
  default: return 6;
  }
}

int main(void) {
  int total = 0;
  for (int n = 0; n < 12; n++) {
    calls = 0;
    accumulate(n, &total);
    printf("%d %d %d\n", n, calls, total);
  }
  printf("%d %d %d %d\n", classify("  a"), classify("\t b"),
         classify("  n"), classify(" x"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn choose(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value % 4
// CHECK-NEXT: }
// CHECK-NEXT: fn accumulate(n: i32, total: *mut i32) {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if i >= n {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                 i = [[V_0]] + 1;
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]]: i32 = choose([[V_0]]);
// CHECK-NEXT:                 if [[T_0]] == 0 {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *total = (unsafe { *total }) + 3;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_0]] == 1 {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             *total = (unsafe { *total }) + 5;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         if [[T_0]] == 2 {} else {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 *total = (unsafe { *total }) - 2;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *total = (unsafe { *total }) + 7;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *total = (unsafe { *total }) + unsafe { calls };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(mut s: *const i8) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let [[T_0]]: i32 = (unsafe { *s }) as i32;
// CHECK-NEXT:             if [[T_0]] == 97 {
// CHECK-NEXT:                 return 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if [[T_0]] == 98 {
// CHECK-NEXT:                     return 2;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_0]] == 99 {
// CHECK-NEXT:                         return 3;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         if [[T_0]] == 100 {
// CHECK-NEXT:                             return 4;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_0]] == 32 {
// CHECK-NEXT:                                 let [[V_1:__v[0-9]+]]: *const i8 = s;
// CHECK-NEXT:                                 s = unsafe { [[V_1]].offset(1 as isize) };
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if [[T_0]] == 9 {
// CHECK-NEXT:                                     let [[V_2:__v[0-9]+]]: *const i8 = s;
// CHECK-NEXT:                                     s = unsafe { [[V_2]].offset(1 as isize) };
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     if [[T_0]] == 110 {
// CHECK-NEXT:                                         return 5;
// CHECK-NEXT:                                     } else {
// CHECK-NEXT:                                         break;
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return 6;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for n in 0 as i32..12 as i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             calls = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         accumulate(n, std::ptr::addr_of_mut!(total));
// CHECK-NEXT:         println!("{} {} {}", n, unsafe { calls }, total);
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", classify(c"  a".as_ptr()), classify(c"\t b".as_ptr()),
// CHECK-NEXT:         classify(c"  n".as_ptr()), classify(c" x".as_ptr())
// CHECK-NEXT:     );
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
