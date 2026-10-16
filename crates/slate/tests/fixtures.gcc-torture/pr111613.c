#include <stdio.h>
#include <stdlib.h>

struct bitfield {
  unsigned int field1 : 1;
  unsigned int field2 : 1;
  unsigned int field3 : 1;
};

__attribute__((noinline)) static void
set_field1_and_field2(struct bitfield *b) {
  b->field1 = 1;
  b->field2 = 1;
}

__attribute__((noinline)) static struct bitfield *new_bitfield(void) {
  struct bitfield *b = (struct bitfield *)malloc(sizeof(*b));
  b->field3          = 1;
  set_field1_and_field2(b);
  return b;
}

int main(void) {
  struct bitfield *b = new_bitfield();
  if (b->field3 != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bitfield {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub field1: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub field2: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub field3: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn set_field1_and_field2(mut b: *mut bitfield) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*b).__slate_bits_0.__set_field1((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*b).__slate_bits_0.__set_field2((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn new_bitfield() -> *mut bitfield {
// CHECK-NEXT:     let mut b: *mut bitfield = (unsafe { malloc(4 as u64) }) as *mut bitfield;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*b).__slate_bits_0.__set_field3((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     set_field1_and_field2(b);
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: *mut bitfield = new_bitfield();
// CHECK-NEXT:     if ((unsafe { (*b).__slate_bits_0.__get_field3() }) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
