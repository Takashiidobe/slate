/* Copyright 2002 Free Software Foundation, Inc.

   Tests correct signedness of operations on bitfields; in particular
   that integer promotions are done correctly, including the case when
   casts are present.

   The C front end was eliding the cast of an unsigned bitfield to
   unsigned as a no-op, when in fact it forces a conversion to a
   full-width unsigned int. (At the time of writing, the C++ front end
   has a different bug; it erroneously promotes the uncast unsigned
   bitfield to an unsigned int).

   Source: Neil Booth, 25 Jan 2002, based on PR 3325 (and 3326, which
   is a different manifestation of the same bug).
*/

extern void abort();

int main(int argc, char *argv[]) {
  struct x {
    signed int   i : 7;
    unsigned int u : 7;
  } bit;

  unsigned int u;
  int          i;
  unsigned int unsigned_result = -13U % 61;
  int          signed_result   = -13 % 61;

  bit.u = 61, u = 61;
  bit.i = -13, i = -13;

  if (i % u != unsigned_result)
    abort();
  if (i % (unsigned int)u != unsigned_result)
    abort();

  /* Somewhat counter-intuitively, bit.u is promoted to an int, making
     the operands and result an int.  */
  if (i % bit.u != signed_result)
    abort();

  if (bit.i % bit.u != signed_result)
    abort();

  /* But with a cast to unsigned int, the unsigned int is promoted to
     itself as a no-op, and the operands and result are unsigned.  */
  if (i % (unsigned int)bit.u != unsigned_result)
    abort();

  if (bit.i % (unsigned int)bit.u != unsigned_result)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright 2002 Free Software Foundation, Inc.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Tests correct signedness of operations on bitfields; in particular
// CHECK-NEXT: //! that integer promotions are done correctly, including the case when
// CHECK-NEXT: //! casts are present.
// CHECK-NEXT: //!
// CHECK-NEXT: //! The C front end was eliding the cast of an unsigned bitfield to
// CHECK-NEXT: //! unsigned as a no-op, when in fact it forces a conversion to a
// CHECK-NEXT: //! full-width unsigned int. (At the time of writing, the C++ front end
// CHECK-NEXT: //! has a different bug; it erroneously promotes the uncast unsigned
// CHECK-NEXT: //! bitfield to an unsigned int).
// CHECK-NEXT: //!
// CHECK-NEXT: //! Source: Neil Booth, 25 Jan 2002, based on PR 3325 (and 3326, which
// CHECK-NEXT: //! is a different manifestation of the same bug).
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct x {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
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
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub i: i32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub u: u32,
// CHECK-NEXT:         #[bits(2, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut bit: x = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut u: u32 = (61 as i32) as u32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut unsigned_result: u32 = (13 as u32).wrapping_neg() % ((61 as i32) as u32);
// CHECK-NEXT:     let mut signed_result: i32 = -(13 as i32) % (61 as i32);
// CHECK-NEXT:     bit.__slate_bits_0.__set_u((61 as i32) as u32);
// CHECK-NEXT:     bit.__slate_bits_0.__set_i(-(13 as i32));
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = -(13 as i32);
// CHECK-NEXT:     i = [[V_0]];
// CHECK-NEXT:     if (i as u32) % u != unsigned_result {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (i as u32) % u != unsigned_result {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // Somewhat counter-intuitively, bit.u is promoted to an int, making
// CHECK-NEXT:     // the operands and result an int.
// CHECK-NEXT:     if i % (bit.__slate_bits_0.__get_u() as i32) != signed_result {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bit.__slate_bits_0.__get_i() % (bit.__slate_bits_0.__get_u() as i32) != signed_result {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // But with a cast to unsigned int, the unsigned int is promoted to
// CHECK-NEXT:     // itself as a no-op, and the operands and result are unsigned.
// CHECK-NEXT:     if (i as u32) % ((bit.__slate_bits_0.__get_u() as i32) as u32) != unsigned_result {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (bit.__slate_bits_0.__get_i() as u32) % ((bit.__slate_bits_0.__get_u() as i32) as u32)
// CHECK-NEXT:         != unsigned_result
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
