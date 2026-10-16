/* PR target/60017 */

extern void abort(void);

struct S0 {
  short m0;
  short m1;
};

struct S1 {
  unsigned  m0 : 1;
  char      m1[2][2];
  struct S0 m2[2];
};

struct S1 x = {1, {{2, 3}, {4, 5}}, {{6, 7}, {8, 9}}};

struct S1 func(void) { return x; }

int main(void) {
  struct S1 ret = func();

  if (ret.m2[1].m1 != 9)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/60017
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S0 {
// CHECK-NEXT:     m0: i16,
// CHECK-NEXT:     m1: i16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S1 {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT:     m1: {{\[\[}}i8; 2]; 2],
// CHECK-NEXT:     m2: [S0; 2],
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
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub m0: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: S1 = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: S1 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_m0((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].m1 = [
// CHECK-NEXT:         [(2 as i32) as i8, (3 as i32) as i8],
// CHECK-NEXT:         [(4 as i32) as i8, (5 as i32) as i8],
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]].m2 = [
// CHECK-NEXT:         S0 {
// CHECK-NEXT:             m0: (6 as i32) as i16,
// CHECK-NEXT:             m1: (7 as i32) as i16,
// CHECK-NEXT:         },
// CHECK-NEXT:         S0 {
// CHECK-NEXT:             m0: (8 as i32) as i16,
// CHECK-NEXT:             m1: (9 as i32) as i16,
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn func() -> S1 {
// CHECK-NEXT:     return unsafe { x };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ret: S1 = func();
// CHECK-NEXT:     if ((unsafe { (*unsafe { (ret.m2.as_mut_ptr() as *mut S0).offset((1 as i32) as isize) }).m1 })
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         != (9 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
