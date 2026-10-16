extern void abort();

#define HOST_WIDE_INT          long
#define HOST_BITS_PER_WIDE_INT (sizeof(long) * 8)

struct tree_type {
  unsigned int precision : 9;
};

int sign_bit_p(struct tree_type *t, HOST_WIDE_INT val_hi,
               unsigned HOST_WIDE_INT val_lo) {
  unsigned HOST_WIDE_INT mask_lo, lo;
  HOST_WIDE_INT          mask_hi, hi;
  int                    width = t->precision;

  if (width > HOST_BITS_PER_WIDE_INT) {
    hi = (unsigned HOST_WIDE_INT)1 << (width - HOST_BITS_PER_WIDE_INT - 1);
    lo = 0;

    mask_hi =
        ((unsigned HOST_WIDE_INT) - 1 >> (2 * HOST_BITS_PER_WIDE_INT - width));
    mask_lo = -1;
  } else {
    hi = 0;
    lo = (unsigned HOST_WIDE_INT)1 << (width - 1);

    mask_hi = 0;
    mask_lo =
        ((unsigned HOST_WIDE_INT) - 1 >> (HOST_BITS_PER_WIDE_INT - width));
  }

  if ((val_hi & mask_hi) == hi && (val_lo & mask_lo) == lo)
    return 1;

  return 0;
}

int main() {
  struct tree_type t;
  t.precision = 1;
  if (!sign_bit_p(&t, 0, -1))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tree_type {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub precision: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sign_bit_p(mut t: &mut tree_type, mut val_hi: i64, mut val_lo: u64) -> i32 {
// CHECK-NEXT:     let mut mask_lo: u64 = 0 as u64;
// CHECK-NEXT:     let mut lo: u64 = 0 as u64;
// CHECK-NEXT:     let mut mask_hi: i64 = 0 as i64;
// CHECK-NEXT:     let mut hi: i64 = 0 as i64;
// CHECK-NEXT:     let mut width: i32 = (unsafe {
// CHECK-NEXT:         (*(t as *mut tree_type)).__slate_bits_0.__get_precision()
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     if ((width as i64) as u64) > (8 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:         hi = ((((1 as i32) as i64) as u64)
// CHECK-NEXT:             << ((width as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub((8 as u64).wrapping_mul(((8 as i32) as i64) as u64))
// CHECK-NEXT:                 .wrapping_sub(((1 as i32) as i64) as u64)) as i64;
// CHECK-NEXT:         lo = ((0 as i32) as i64) as u64;
// CHECK-NEXT:         mask_hi = (((-(1 as i32) as i64) as u64)
// CHECK-NEXT:             >> (((2 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_mul((8 as u64).wrapping_mul(((8 as i32) as i64) as u64))
// CHECK-NEXT:                 .wrapping_sub((width as i64) as u64)) as i64;
// CHECK-NEXT:         mask_lo = (-(1 as i32) as i64) as u64;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         hi = (0 as i32) as i64;
// CHECK-NEXT:         lo = (((1 as i32) as i64) as u64) << width - (1 as i32);
// CHECK-NEXT:         mask_hi = (0 as i32) as i64;
// CHECK-NEXT:         mask_lo = ((-(1 as i32) as i64) as u64)
// CHECK-NEXT:             >> (8 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub((width as i64) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if val_hi & mask_hi == hi && val_lo & mask_lo == lo {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: tree_type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     t.__slate_bits_0.__set_precision((1 as i32) as u32);
// CHECK-NEXT:     if !(sign_bit_p(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(t)) },
// CHECK-NEXT:         (0 as i32) as i64,
// CHECK-NEXT:         (-(1 as i32) as i64) as u64,
// CHECK-NEXT:     ) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
