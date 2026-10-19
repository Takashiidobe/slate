#include <stdio.h>

int counter = 4;
int zeroed;
int numbers[4] = {1, 2};

struct Pair {
  int left;
  int right;
};

struct Pair pair = {3, 5};

static int adjust(int by) {
  counter    += by;
  zeroed     += counter;
  numbers[2]  = zeroed - numbers[0];
  pair.right += numbers[1];
  return pair.left + pair.right;
}

int main(void) {
  printf("%d\n", adjust(6));
  printf("%d %d %d\n", counter, zeroed, numbers[2]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = 4;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut zeroed: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut numbers: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = 1i32;
// CHECK-NEXT:     [[T_0]][1] = 2i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut pair: Pair = Pair { left: 3i32, right: 5i32 };
// CHECK-NEXT: fn adjust(by: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = counter + by;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         zeroed = zeroed + counter;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         numbers.0[2] = zeroed - numbers.0[0];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pair.right = pair.right + numbers.0[1];
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { pair.left }) + unsafe { pair.right }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", adjust(6i32));
// CHECK-NEXT:     println!("{} {} {}", unsafe { counter }, unsafe { zeroed }, unsafe { numbers.0[2] });
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
