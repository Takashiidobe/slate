/* { dg-options "-mms-bitfields" } */
int printf(const char *, ...);

struct widening {
  int x : 3;
  long long y : 3;
  int z : 3;
  char tail;
};

int main(void) {
  struct widening pair[2] = {{1, 2, 3, 4}, {1, 2, 3, 4}};
  printf("%d %d\n", (int)((char *)&pair[1] - (char *)&pair[0]), (int)((char *)&pair[0].tail - (char *)&pair[0]));
  printf("%d %d %d %d\n", pair[1].x, (int)pair[1].y, pair[1].z, pair[1].tail);
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-mms-bitfields" }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct widening {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     __slate_pad_1: [u8; 4],
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits0U1,
// CHECK-NEXT:     __slate_bits_2: __slate_bits::__SlateBits0U2,
// CHECK-NEXT:     tail: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
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
// CHECK-NEXT:     pub struct __SlateBits0U1 {
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
// CHECK-NEXT:     pub struct __SlateBits0U2 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub z: i32,
// CHECK-NEXT:         #[bits(29, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut pair: [widening; 2] = [
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: widening = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_x(1i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_1.__set_y(2i64);
// CHECK-NEXT:             [[T_0]].__slate_bits_2.__set_z(3i32);
// CHECK-NEXT:             [[T_0]].tail = 4i8;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: widening = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].__slate_bits_0.__set_x(1i32);
// CHECK-NEXT:             [[T_1]].__slate_bits_1.__set_y(2i64);
// CHECK-NEXT:             [[T_1]].__slate_bits_2.__set_z(3i32);
// CHECK-NEXT:             [[T_1]].tail = 4i8;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr(),
// CHECK-NEXT:             ((pair.as_mut_ptr() as *mut widening).add(1) as *mut i8)
// CHECK-NEXT:                 .offset_from((pair.as_mut_ptr() as *mut widening).add(0) as *mut i8)
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             std::ptr::addr_of_mut!(pair[0].tail)
// CHECK-NEXT:                 .offset_from((pair.as_mut_ptr() as *mut widening).add(0) as *mut i8)
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             pair[1].__slate_bits_0.__get_x(),
// CHECK-NEXT:             pair[1].__slate_bits_1.__get_y() as i32,
// CHECK-NEXT:             pair[1].__slate_bits_2.__get_z(),
// CHECK-NEXT:             pair[1].tail as i32,
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
