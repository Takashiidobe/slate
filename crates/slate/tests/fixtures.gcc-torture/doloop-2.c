#include <limits.h>

extern void exit(int);
extern void abort(void);

volatile unsigned int i;

int main(void) {
  unsigned short z = 0;

  do
    ++i;
  while (--z > 0);
  if (i != USHRT_MAX + 1U)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut z: u16 = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), [[V_2]]) };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u16 = z;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u16 = (((([[V_3]] as u32) as i32) - (1 as i32)) as i16) as u16;
// CHECK-NEXT:         z = [[V_4]];
// CHECK-NEXT:         if !((([[V_4]] as u32) as i32) > (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) })
// CHECK-NEXT:         != (((32767 as i32) * (2 as i32) + (1 as i32)) as u32).wrapping_add(1 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
