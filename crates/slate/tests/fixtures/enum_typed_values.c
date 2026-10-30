#include <stdio.h>

enum Color { Red, Green = 4, Blue };
enum Signed { Low = -3, Mid, High = 7 };
enum Small : unsigned char { Tiny = 1, Huge = 250 };
typedef enum { Off, On } Switch;

struct Lamp {
  Switch state;
  enum Color color;
  enum Small size;
};

static enum Color favorite = Blue;
static enum Signed levels[3] = {Low, Mid, High};

static enum Color next(enum Color color) {
  if (color == Red)
    return Green;
  if (color == Green)
    return Blue;
  return Red;
}

static Switch flip(Switch value) {
  if (value == On)
    return Off;
  return On;
}

int main(void) {
  struct Lamp lamp = {On, Green, Huge};
  enum Color color = Red;
  int sum = 0;
  for (int i = 0; i < 4; i++) {
    color = next(color);
    sum += color;
  }
  lamp.state = flip(lamp.state);
  lamp.color = next(lamp.color);
  enum Small size = lamp.size;
  size = (enum Small)(size + 3);
  printf("%d %d %d %d\n", sum, lamp.state, lamp.color, size);
  printf("%d %d %d %d\n", favorite, levels[0], levels[1], levels[2]);
  printf("%zu %zu %zu\n", sizeof(enum Color), sizeof(enum Small), sizeof(struct Lamp));
  return levels[0] + favorite;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Lamp {
// CHECK-NEXT:     state: u32,
// CHECK-NEXT:     color: u32,
// CHECK-NEXT:     size: u8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut favorite: u32 = 5;
// CHECK-NEXT: static mut levels: [i32; 3] = [-3, -2, 7];
// CHECK-NEXT: fn next(color: u32) -> u32 {
// CHECK-NEXT:     if (color as u32) == 0 {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (color as u32) == 4 {
// CHECK-NEXT:         return 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn flip(value: u32) -> u32 {
// CHECK-NEXT:     if (value as u32) == 1 {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut lamp: Lamp = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Lamp = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].state = 1u32;
// CHECK-NEXT:         [[T_0]].color = 4u32;
// CHECK-NEXT:         [[T_0]].size = 250u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut color: u32 = 0;
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         color = next(color);
// CHECK-NEXT:         sum = (sum as u32).wrapping_add(color as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     lamp.state = flip(lamp.state);
// CHECK-NEXT:     lamp.color = next(lamp.color);
// CHECK-NEXT:     let mut size: u8 = lamp.size;
// CHECK-NEXT:     size = (((((size as u8) as u32) as i32) + 3) as i8) as u8;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", sum, lamp.state as u32, lamp.color as u32, ((size as u8) as u32)
// CHECK-NEXT:         as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", (unsafe { favorite }) as u32, (unsafe { levels[0] }) as i32,
// CHECK-NEXT:         (unsafe { levels[1] }) as i32, (unsafe { levels[2] }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu\n".as_ptr(), 4u64, 1u64, 12u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     (((unsafe { levels[0] }) as i32) as u32).wrapping_add((unsafe { favorite }) as u32)
// CHECK-NEXT:         as i32
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
