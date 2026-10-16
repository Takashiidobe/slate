/*
 This test checks promotion of bitfields.  Bitfields should be promoted
 very much like chars and shorts:

 Bitfields (signed or unsigned) should be promoted to signed int if their
 value will fit in a signed int, otherwise to an unsigned int if their
 value will fit in an unsigned int, otherwise we don't promote them (ANSI/ISO
 does not specify the behavior of bitfields larger than an unsigned int).

 We test the behavior by subtracting two from the promoted value: this will
 result in a negitive value for signed types, a positive value for unsigned
 types.  This test (of course) assumes that the compiler is correctly
 implementing signed and unsigned arithmetic.
 */

void abort(void);
void exit(int);

struct X {
  unsigned int       u3    : 3;
  signed long int    s31   : 31;
  signed long int    s32   : 32;
  unsigned long int  u31   : 31;
  unsigned long int  u32   : 32;
  unsigned long long ull3  : 3;
  unsigned long long ull35 : 35;
  unsigned           u15   : 15;
};

struct X x;

int main(void) {
  if ((x.u3 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.s31 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.s32 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.u15 - 2) >= 0) /* promoted value should be signed */
    abort();

  /* Conditionalize check on whether integers are 4 bytes or larger, i.e.
     larger than a 31 bit bitfield.  */
  if (sizeof(int) >= 4) {
    if ((x.u31 - 2) >= 0) /* promoted value should be signed */
      abort();
  } else {
    if ((x.u31 - 2) < 0) /* promoted value should be UNsigned */
      abort();
  }

  if ((x.u32 - 2) < 0) /* promoted value should be UNsigned */
    abort();

  if ((x.ull3 - 2) >= 0) /* promoted value should be signed */
    abort();

  if ((x.ull35 - 2) < 0) /* promoted value should be UNsigned */
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This test checks promotion of bitfields.  Bitfields should be promoted
// CHECK-NEXT: //! very much like chars and shorts:
// CHECK-NEXT: //!
// CHECK-NEXT: //! Bitfields (signed or unsigned) should be promoted to signed int if their
// CHECK-NEXT: //! value will fit in a signed int, otherwise to an unsigned int if their
// CHECK-NEXT: //! value will fit in an unsigned int, otherwise we don't promote them (ANSI/ISO
// CHECK-NEXT: //! does not specify the behavior of bitfields larger than an unsigned int).
// CHECK-NEXT: //!
// CHECK-NEXT: //! We test the behavior by subtracting two from the promoted value: this will
// CHECK-NEXT: //! result in a negitive value for signed types, a positive value for unsigned
// CHECK-NEXT: //! types.  This test (of course) assumes that the compiler is correctly
// CHECK-NEXT: //! implementing signed and unsigned arithmetic.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct X {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits0U1,
// CHECK-NEXT:     __slate_bits_2: __slate_bits::__SlateBits0U2,
// CHECK-NEXT:     __slate_pad_3: [u8; 3],
// CHECK-NEXT:     __slate_bits_3: __slate_bits::__SlateBits0U3,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 5], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub u3: u32,
// CHECK-NEXT:         #[bits(31)]
// CHECK-NEXT:         pub s31: i64,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
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
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub s32: i64,
// CHECK-NEXT:         #[bits(31)]
// CHECK-NEXT:         pub u31: u64,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield([u8; 5], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U2 {
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub u32: u64,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub ull3: u64,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield([u8; 7], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U3 {
// CHECK-NEXT:         #[bits(35)]
// CHECK-NEXT:         pub ull35: u64,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub u15: u32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: X = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_0)).__get_u3() }) as i32) - (2 as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be signed
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_0)).__get_s31() }) as i32) - (2 as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be signed
// CHECK-NEXT:     if (unsafe { (*std::ptr::addr_of!(x.__slate_bits_1)).__get_s32() }) - ((2 as i32) as i64)
// CHECK-NEXT:         >= ((0 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be signed
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_3)).__get_u15() }) as i32) - (2 as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be signed
// CHECK-NEXT:     // Conditionalize check on whether integers are 4 bytes or larger, i.e.
// CHECK-NEXT:     // larger than a 31 bit bitfield.
// CHECK-NEXT:     if (4 as u64) >= (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         if (((unsafe { (*std::ptr::addr_of!(x.__slate_bits_1)).__get_u31() }) as u32) as i32)
// CHECK-NEXT:             - (2 as i32)
// CHECK-NEXT:             >= (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     // promoted value should be signed
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if (((unsafe { (*std::ptr::addr_of!(x.__slate_bits_1)).__get_u31() }) as u32) as i32)
// CHECK-NEXT:             - (2 as i32)
// CHECK-NEXT:             < (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         // promoted value should be UNsigned
// CHECK-NEXT:     }
// CHECK-NEXT:     if unsafe { (*std::ptr::addr_of!(x.__slate_bits_2)).__get_u32() }
// CHECK-NEXT:         .wrapping_sub(((2 as i32) as i64) as u64)
// CHECK-NEXT:         < (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be UNsigned
// CHECK-NEXT:     if (((unsafe { (*std::ptr::addr_of!(x.__slate_bits_2)).__get_ull3() }) as u32) as i32)
// CHECK-NEXT:         - (2 as i32)
// CHECK-NEXT:         >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be signed
// CHECK-NEXT:     if unsafe { (*std::ptr::addr_of!(x.__slate_bits_3)).__get_ull35() }
// CHECK-NEXT:         .wrapping_sub(((2 as i32) as i64) as u64)
// CHECK-NEXT:         < (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // promoted value should be UNsigned
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
