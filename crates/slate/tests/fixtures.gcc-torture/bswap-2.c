/* { dg-require-effective-target int32plus } */

#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef __UINT32_TYPE__ unsigned;
#endif

struct bitfield {
  unsigned char f0 : 7;
  unsigned char    : 1;
  unsigned char f1 : 7;
  unsigned char    : 1;
  unsigned char f2 : 7;
  unsigned char    : 1;
  unsigned char f3 : 7;
};

struct ok {
  unsigned char f0;
  unsigned char f1;
  unsigned char f2;
  unsigned char f3;
};

union bf_or_uint32 {
  struct ok       inval;
  struct bitfield bfval;
};

__attribute__((noinline, noclone)) uint32_t
partial_read_le32(union bf_or_uint32 in) {
  return in.bfval.f0 | (in.bfval.f1 << 8) | (in.bfval.f2 << 16) |
         (in.bfval.f3 << 24);
}

__attribute__((noinline, noclone)) uint32_t
partial_read_be32(union bf_or_uint32 in) {
  return in.bfval.f3 | (in.bfval.f2 << 8) | (in.bfval.f1 << 16) |
         (in.bfval.f0 << 24);
}

__attribute__((noinline, noclone)) uint32_t fake_read_le32(char *x, char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  *y = 1;
  c2 = x[2];
  c3 = x[3];
  return c0 | c1 << 8 | c2 << 16 | c3 << 24;
}

__attribute__((noinline, noclone)) uint32_t fake_read_be32(char *x, char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  *y = 1;
  c2 = x[2];
  c3 = x[3];
  return c3 | c2 << 8 | c1 << 16 | c0 << 24;
}

__attribute__((noinline, noclone)) uint32_t incorrect_read_le32(char *x,
                                                                char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  c2 = x[2];
  c3 = x[3];
  *y = 1;
  return c0 | c1 << 8 | c2 << 16 | c3 << 24;
}

__attribute__((noinline, noclone)) uint32_t incorrect_read_be32(char *x,
                                                                char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  c2 = x[2];
  c3 = x[3];
  *y = 1;
  return c3 | c2 << 8 | c1 << 16 | c0 << 24;
}

int main() {
  union bf_or_uint32 bfin;
  uint32_t           out;
  char               cin[] = {0x83, 0x85, 0x87, 0x89};

  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;
  bfin.inval = (struct ok){0x83, 0x85, 0x87, 0x89};
  out        = partial_read_le32(bfin);
  /* Test what bswap would do if its check are not strict enough instead of
     what is the expected result as there is too many possible results with
     bitfields.  */
  if (out == 0x89878583)
    __builtin_abort();
  bfin.inval = (struct ok){0x83, 0x85, 0x87, 0x89};
  out        = partial_read_be32(bfin);
  /* Test what bswap would do if its check are not strict enough instead of
     what is the expected result as there is too many possible results with
     bitfields.  */
  if (out == 0x83858789)
    __builtin_abort();
  out = fake_read_le32(cin, &cin[2]);
  if (out != 0x89018583)
    __builtin_abort();
  cin[2] = 0x87;
  out    = fake_read_be32(cin, &cin[2]);
  if (out != 0x83850189)
    __builtin_abort();
  cin[2] = 0x87;
  out    = incorrect_read_le32(cin, &cin[2]);
  if (out != 0x89878583)
    __builtin_abort();
  cin[2] = 0x87;
  out    = incorrect_read_be32(cin, &cin[2]);
  if (out != 0x83858789)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bitfield {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ok {
// CHECK-NEXT:     f0: u8,
// CHECK-NEXT:     f1: u8,
// CHECK-NEXT:     f2: u8,
// CHECK-NEXT:     f3: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union bf_or_uint32 {
// CHECK-NEXT:     inval: ok,
// CHECK-NEXT:     bfval: bitfield,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 4], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub f0: u8,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub f1: u8,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub f2: u8,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_5: u8,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub f3: u8,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_7: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn partial_read_le32(mut r#in: bf_or_uint32) -> u32 {
// CHECK-NEXT:     ((((unsafe { r#in.bfval.__slate_bits_0.__get_f0() }) as u32) as i32)
// CHECK-NEXT:         | (((unsafe { r#in.bfval.__slate_bits_0.__get_f1() }) as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | (((unsafe { r#in.bfval.__slate_bits_0.__get_f2() }) as u32) as i32) << (16 as i32)
// CHECK-NEXT:         | (((unsafe { r#in.bfval.__slate_bits_0.__get_f3() }) as u32) as i32) << (24 as i32))
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn partial_read_be32(mut r#in: bf_or_uint32) -> u32 {
// CHECK-NEXT:     ((((unsafe { r#in.bfval.__slate_bits_0.__get_f3() }) as u32) as i32)
// CHECK-NEXT:         | (((unsafe { r#in.bfval.__slate_bits_0.__get_f2() }) as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | (((unsafe { r#in.bfval.__slate_bits_0.__get_f1() }) as u32) as i32) << (16 as i32)
// CHECK-NEXT:         | (((unsafe { r#in.bfval.__slate_bits_0.__get_f0() }) as u32) as i32) << (24 as i32))
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fake_read_le32(mut x: *mut i8, mut y: *mut i8) -> u32 {
// CHECK-NEXT:     let mut c0: u8 = (unsafe { *unsafe { x.offset((0 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c1: u8 = (unsafe { *unsafe { x.offset((1 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c2: u8 = 0 as u8;
// CHECK-NEXT:     let mut c3: u8 = 0 as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     c2 = (unsafe { *unsafe { x.offset((2 as i32) as isize) } }) as u8;
// CHECK-NEXT:     c3 = (unsafe { *unsafe { x.offset((3 as i32) as isize) } }) as u8;
// CHECK-NEXT:     (((c0 as u32) as i32)
// CHECK-NEXT:         | ((c1 as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | ((c2 as u32) as i32) << (16 as i32)
// CHECK-NEXT:         | ((c3 as u32) as i32) << (24 as i32)) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fake_read_be32(mut x: *mut i8, mut y: *mut i8) -> u32 {
// CHECK-NEXT:     let mut c0: u8 = (unsafe { *unsafe { x.offset((0 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c1: u8 = (unsafe { *unsafe { x.offset((1 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c2: u8 = 0 as u8;
// CHECK-NEXT:     let mut c3: u8 = 0 as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     c2 = (unsafe { *unsafe { x.offset((2 as i32) as isize) } }) as u8;
// CHECK-NEXT:     c3 = (unsafe { *unsafe { x.offset((3 as i32) as isize) } }) as u8;
// CHECK-NEXT:     (((c3 as u32) as i32)
// CHECK-NEXT:         | ((c2 as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | ((c1 as u32) as i32) << (16 as i32)
// CHECK-NEXT:         | ((c0 as u32) as i32) << (24 as i32)) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn incorrect_read_le32(mut x: *mut i8, mut y: *mut i8) -> u32 {
// CHECK-NEXT:     let mut c0: u8 = (unsafe { *unsafe { x.offset((0 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c1: u8 = (unsafe { *unsafe { x.offset((1 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c2: u8 = (unsafe { *unsafe { x.offset((2 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c3: u8 = (unsafe { *unsafe { x.offset((3 as i32) as isize) } }) as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     (((c0 as u32) as i32)
// CHECK-NEXT:         | ((c1 as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | ((c2 as u32) as i32) << (16 as i32)
// CHECK-NEXT:         | ((c3 as u32) as i32) << (24 as i32)) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn incorrect_read_be32(mut x: *mut i8, mut y: *mut i8) -> u32 {
// CHECK-NEXT:     let mut c0: u8 = (unsafe { *unsafe { x.offset((0 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c1: u8 = (unsafe { *unsafe { x.offset((1 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c2: u8 = (unsafe { *unsafe { x.offset((2 as i32) as isize) } }) as u8;
// CHECK-NEXT:     let mut c3: u8 = (unsafe { *unsafe { x.offset((3 as i32) as isize) } }) as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     (((c3 as u32) as i32)
// CHECK-NEXT:         | ((c2 as u32) as i32) << (8 as i32)
// CHECK-NEXT:         | ((c1 as u32) as i32) << (16 as i32)
// CHECK-NEXT:         | ((c0 as u32) as i32) << (24 as i32)) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: ok = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: ok = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut bfin: bf_or_uint32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut out: u32 = 0 as u32;
// CHECK-NEXT:     let mut cin: [i8; 4] = [
// CHECK-NEXT:         (131 as i32) as i8,
// CHECK-NEXT:         (133 as i32) as i8,
// CHECK-NEXT:         (135 as i32) as i8,
// CHECK-NEXT:         (137 as i32) as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     if (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((32 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         bfin.inval = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = ok {
// CHECK-NEXT:                     f0: ((131 as i32) as i8) as u8,
// CHECK-NEXT:                     f1: ((133 as i32) as i8) as u8,
// CHECK-NEXT:                     f2: ((135 as i32) as i8) as u8,
// CHECK-NEXT:                     f3: ((137 as i32) as i8) as u8,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     out = partial_read_le32(bfin);
// CHECK-NEXT:     // Test what bswap would do if its check are not strict enough instead of
// CHECK-NEXT:     // what is the expected result as there is too many possible results with
// CHECK-NEXT:     // bitfields.
// CHECK-NEXT:     if out == (2307360131 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         bfin.inval = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = ok {
// CHECK-NEXT:                     f0: ((131 as i32) as i8) as u8,
// CHECK-NEXT:                     f1: ((133 as i32) as i8) as u8,
// CHECK-NEXT:                     f2: ((135 as i32) as i8) as u8,
// CHECK-NEXT:                     f3: ((137 as i32) as i8) as u8,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     out = partial_read_be32(bfin);
// CHECK-NEXT:     // Test what bswap would do if its check are not strict enough instead of
// CHECK-NEXT:     // what is the expected result as there is too many possible results with
// CHECK-NEXT:     // bitfields.
// CHECK-NEXT:     if out == (2206566281 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     out = fake_read_le32(cin.as_mut_ptr() as *mut i8, unsafe {
// CHECK-NEXT:         (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     if out != (2298578307 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } = (135 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     out = fake_read_be32(cin.as_mut_ptr() as *mut i8, unsafe {
// CHECK-NEXT:         (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     if out != (2206531977 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } = (135 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     out = incorrect_read_le32(cin.as_mut_ptr() as *mut i8, unsafe {
// CHECK-NEXT:         (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     if out != (2307360131 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize) } = (135 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     out = incorrect_read_be32(cin.as_mut_ptr() as *mut i8, unsafe {
// CHECK-NEXT:         (cin.as_mut_ptr() as *mut i8).offset((2 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     if out != (2206566281 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
