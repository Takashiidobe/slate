#include <stdio.h>

static int jump_probe(int x) {
  static void *targets[] = {&&zero, &&one, &&two};
  if (x < 0 || x > 2) {
    return -1;
  }
  goto *targets[x];

zero:
  return 10;
one:
  return 20;
two:
  return 30;
}

int main(void) {
  volatile int input = 2;
  printf("%d\n", jump_probe(input));
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
// CHECK-NEXT: fn jump_probe(x: i32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 5usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 30;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 20;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     return 10;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     match targets.0[x as usize] as usize {
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 2usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         6 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     return -1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     if x < 0 || x > 2 {
// CHECK-NEXT:                         __slate_state = 4usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: static mut targets: __SlateAlign16<[*mut (); 3]> = __SlateAlign16([
// CHECK-NEXT:     4usize as *mut (),
// CHECK-NEXT:     5usize as *mut (),
// CHECK-NEXT:     6usize as *mut (),
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let input: i32 = 2;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", jump_probe(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) })
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
