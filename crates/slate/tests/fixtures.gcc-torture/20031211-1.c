void abort(void);
void exit(int);

struct a {
  unsigned int bitfield : 1;
};

unsigned int x;

int main(void) {
  struct a a  = {0};
  x           = 0xbeef;
  a.bitfield |= x;
  if (a.bitfield != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct a {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bitfield: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: a = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: a = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_bitfield((0 as i32) as u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = (48879 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = a.__slate_bits_0.__get_bitfield();
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = (([[V_1]] as i32) as u32) | unsafe { x };
// CHECK-NEXT:     a.__slate_bits_0.__set_bitfield([[V_2]]);
// CHECK-NEXT:     if (a.__slate_bits_0.__get_bitfield() as i32) != (1 as i32) {
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
