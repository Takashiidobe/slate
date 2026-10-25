/* { dg-options "-mms-bitfields" } */
int printf(const char *, ...);

struct mixed_units {
  char a : 3;
  int b : 4;
  char c;
};

struct widening {
  int x : 3;
  long long y : 3;
  int z : 3;
};

int main(void) {
  struct mixed_units m = {1, 2, 3};
  struct widening w = {1, 2, 3};
  printf("%zu %zu\n", sizeof(struct mixed_units), sizeof(struct widening));
  printf("%d %d %d %d %d %d\n", m.a, m.b, m.c, w.x, (int)w.y, w.z);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-mms-bitfields" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mixed_units {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits0U1,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct widening {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits1U1,
// CHECK-NEXT:     __slate_bits_2: __slate_bits::__SlateBits1U2,
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i8,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U1 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(28, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
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
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub x: i32,
// CHECK-NEXT:         #[bits(29, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U1 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub y: i64,
// CHECK-NEXT:         #[bits(61, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u64,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U2 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub z: i32,
// CHECK-NEXT:         #[bits(29, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut m: mixed_units = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: mixed_units = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_a(1i8);
// CHECK-NEXT:         [[T_0]].__slate_bits_1.__set_b(2i32);
// CHECK-NEXT:         [[T_0]].c = 3i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut w: widening = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: widening = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_x(1i32);
// CHECK-NEXT:         [[T_1]].__slate_bits_1.__set_y(2i64);
// CHECK-NEXT:         [[T_1]].__slate_bits_2.__set_z(3i32);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 12u64, 24u64) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 12u32, 24u32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             m.__slate_bits_0.__get_a() as i32,
// CHECK-NEXT:             m.__slate_bits_1.__get_b(),
// CHECK-NEXT:             m.c as i32,
// CHECK-NEXT:             w.__slate_bits_0.__get_x(),
// CHECK-NEXT:             w.__slate_bits_1.__get_y() as i32,
// CHECK-NEXT:             w.__slate_bits_2.__get_z(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
