/* PR 16341 */
/* { dg-require-effective-target int32plus } */

void abort(void);
void exit(int);

#define PART_PRECISION (sizeof(cpp_num_part) * 8)

typedef unsigned int   cpp_num_part;
typedef struct cpp_num cpp_num;
struct cpp_num {
  cpp_num_part high;
  cpp_num_part low;
  int          unsignedp; /* True if value should be treated as unsigned.  */
  int          overflow;  /* True if the most recent calculation overflowed.  */
};

static int num_positive(cpp_num num, unsigned int precision) {
  if (precision > PART_PRECISION) {
    precision -= PART_PRECISION;
    return (num.high & (cpp_num_part)1 << (precision - 1)) == 0;
  }

  return (num.low & (cpp_num_part)1 << (precision - 1)) == 0;
}

static cpp_num num_trim(cpp_num num, unsigned int precision) {
  if (precision > PART_PRECISION) {
    precision -= PART_PRECISION;
    if (precision < PART_PRECISION)
      num.high &= ((cpp_num_part)1 << precision) - 1;
  } else {
    if (precision < PART_PRECISION)
      num.low &= ((cpp_num_part)1 << precision) - 1;
    num.high = 0;
  }

  return num;
}

/* Shift NUM, of width PRECISION, right by N bits.  */
static cpp_num num_rshift(cpp_num num, unsigned int precision, unsigned int n) {
  cpp_num_part sign_mask;
  int          x = num_positive(num, precision);

  if (num.unsignedp || x)
    sign_mask = 0;
  else
    sign_mask = ~(cpp_num_part)0;

  if (n >= precision)
    num.high = num.low = sign_mask;
  else {
    /* Sign-extend.  */
    if (precision < PART_PRECISION)
      num.high = sign_mask, num.low |= sign_mask << precision;
    else if (precision < 2 * PART_PRECISION)
      num.high |= sign_mask << (precision - PART_PRECISION);

    if (n >= PART_PRECISION) {
      n        -= PART_PRECISION;
      num.low   = num.high;
      num.high  = sign_mask;
    }

    if (n) {
      num.low  = (num.low >> n) | (num.high << (PART_PRECISION - n));
      num.high = (num.high >> n) | (sign_mask << (PART_PRECISION - n));
    }
  }

  num          = num_trim(num, precision);
  num.overflow = 0;
  return num;
}
#define num_zerop(num)     ((num.low | num.high) == 0)
#define num_eq(num1, num2) (num1.low == num2.low && num1.high == num2.high)

cpp_num num_lshift(cpp_num num, unsigned int precision, unsigned int n) {
  if (n >= precision) {
    num.overflow = !num.unsignedp && !num_zerop(num);
    num.high = num.low = 0;
  } else {
    cpp_num      orig;
    unsigned int m = n;

    orig = num;
    if (m >= PART_PRECISION) {
      m        -= PART_PRECISION;
      num.high  = num.low;
      num.low   = 0;
    }
    if (m) {
      num.high   = (num.high << m) | (num.low >> (PART_PRECISION - m));
      num.low  <<= m;
    }
    num = num_trim(num, precision);

    if (num.unsignedp)
      num.overflow = 0;
    else {
      cpp_num maybe_orig = num_rshift(num, precision, n);
      num.overflow       = !num_eq(orig, maybe_orig);
    }
  }

  return num;
}

unsigned int precision = 64;
unsigned int n         = 16;

cpp_num num = {0, 3, 0, 0};

int main() {
  cpp_num res = num_lshift(num, 64, n);

  if (res.low != 0x30000)
    abort();

  if (res.high != 0)
    abort();

  if (res.overflow != 0)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 16341
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct cpp_num {
// CHECK-NEXT:     high: u32,
// CHECK-NEXT:     low: u32,
// CHECK-NEXT:     /// True if value should be treated as unsigned.
// CHECK-NEXT:     unsignedp: i32,
// CHECK-NEXT:     /// True if the most recent calculation overflowed.
// CHECK-NEXT:     overflow: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn num_positive(mut [[NUM_0:num_[0-9]+]]: cpp_num, mut [[PRECISION_0:precision_[0-9]+]]: u32) -> i32 {
// CHECK-NEXT:     if ([[PRECISION_0]] as u64) > (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[PRECISION_0]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 =
// CHECK-NEXT:             ([[V_1]] as u64).wrapping_sub((4 as u64).wrapping_mul(((8 as i32) as i64) as u64)) as u32;
// CHECK-NEXT:         [[PRECISION_0]] = [[V_2]];
// CHECK-NEXT:         return ([[NUM_0]].high & ((1 as i32) as u32) << [[PRECISION_0]].wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:             == ((0 as i32) as u32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     ([[NUM_0]].low & ((1 as i32) as u32) << [[PRECISION_0]].wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:         == ((0 as i32) as u32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn num_trim(mut [[NUM_1:num_[0-9]+]]: cpp_num, mut [[PRECISION_1:precision_[0-9]+]]: u32) -> cpp_num {
// CHECK-NEXT:     if ([[PRECISION_1]] as u64) > (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[PRECISION_1]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 =
// CHECK-NEXT:             ([[V_3]] as u64).wrapping_sub((4 as u64).wrapping_mul(((8 as i32) as i64) as u64)) as u32;
// CHECK-NEXT:         [[PRECISION_1]] = [[V_4]];
// CHECK-NEXT:         if ([[PRECISION_1]] as u64) < (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: u32 = [[NUM_1]].high;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: u32 =
// CHECK-NEXT:                 [[V_5]] & (((1 as i32) as u32) << [[PRECISION_1]]).wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:             [[NUM_1]].high = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ([[PRECISION_1]] as u64) < (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: u32 = [[NUM_1]].low;
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: u32 =
// CHECK-NEXT:                 [[V_7]] & (((1 as i32) as u32) << [[PRECISION_1]]).wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:             [[NUM_1]].low = [[V_8]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[NUM_1]].high = (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     return [[NUM_1]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Shift NUM, of width PRECISION, right by N bits.
// CHECK-NEXT: fn num_rshift(mut [[NUM_2:num_[0-9]+]]: cpp_num, mut [[PRECISION_2:precision_[0-9]+]]: u32, mut [[N_0:n_[0-9]+]]: u32) -> cpp_num {
// CHECK-NEXT:     let mut sign_mask: u32 = 0 as u32;
// CHECK-NEXT:     let mut x: i32 = num_positive([[NUM_2]], [[PRECISION_2]]);
// CHECK-NEXT:     if [[NUM_2]].unsignedp != (0 as i32) || x != (0 as i32) {
// CHECK-NEXT:         sign_mask = (0 as i32) as u32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         sign_mask = !((0 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[N_0]] >= [[PRECISION_2]] {
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u32 = sign_mask;
// CHECK-NEXT:         [[NUM_2]].low = [[V_9]];
// CHECK-NEXT:         [[NUM_2]].high = [[V_9]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         // Sign-extend.
// CHECK-NEXT:         if ([[PRECISION_2]] as u64) < (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:             [[NUM_2]].high = sign_mask;
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: u32 = [[NUM_2]].low;
// CHECK-NEXT:             let mut [[V_11:__v[0-9]+]]: u32 = [[V_10]] | sign_mask << [[PRECISION_2]];
// CHECK-NEXT:             [[NUM_2]].low = [[V_11]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if ([[PRECISION_2]] as u64)
// CHECK-NEXT:                 < (((2 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_mul((4 as u64).wrapping_mul(((8 as i32) as i64) as u64))
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut [[V_12:__v[0-9]+]]: u32 = [[NUM_2]].high;
// CHECK-NEXT:                 let mut [[V_13:__v[0-9]+]]: u32 = [[V_12]]
// CHECK-NEXT:                     | sign_mask
// CHECK-NEXT:                         << ([[PRECISION_2]] as u64)
// CHECK-NEXT:                             .wrapping_sub((4 as u64).wrapping_mul(((8 as i32) as i64) as u64));
// CHECK-NEXT:                 [[NUM_2]].high = [[V_13]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if ([[N_0]] as u64) >= (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:             let mut [[V_14:__v[0-9]+]]: u32 = [[N_0]];
// CHECK-NEXT:             let mut [[V_15:__v[0-9]+]]: u32 = ([[V_14]] as u64)
// CHECK-NEXT:                 .wrapping_sub((4 as u64).wrapping_mul(((8 as i32) as i64) as u64))
// CHECK-NEXT:                 as u32;
// CHECK-NEXT:             [[N_0]] = [[V_15]];
// CHECK-NEXT:             [[NUM_2]].low = [[NUM_2]].high;
// CHECK-NEXT:             [[NUM_2]].high = sign_mask;
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[N_0]] != (0 as u32) {
// CHECK-NEXT:             [[NUM_2]].low = [[NUM_2]].low >> [[N_0]]
// CHECK-NEXT:                 | [[NUM_2]].high
// CHECK-NEXT:                     << (4 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub([[N_0]] as u64);
// CHECK-NEXT:             [[NUM_2]].high = [[NUM_2]].high >> [[N_0]]
// CHECK-NEXT:                 | sign_mask
// CHECK-NEXT:                     << (4 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub([[N_0]] as u64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     [[NUM_2]] = num_trim([[NUM_2]], [[PRECISION_2]]);
// CHECK-NEXT:     [[NUM_2]].overflow = 0 as i32;
// CHECK-NEXT:     return [[NUM_2]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn num_lshift(mut [[NUM_3:num_[0-9]+]]: cpp_num, mut [[PRECISION_3:precision_[0-9]+]]: u32, mut [[N_1:n_[0-9]+]]: u32) -> cpp_num {
// CHECK-NEXT:     if [[N_1]] >= [[PRECISION_3]] {
// CHECK-NEXT:         [[NUM_3]].overflow = (!([[NUM_3]].unsignedp != (0 as i32))
// CHECK-NEXT:             && !([[NUM_3]].low | [[NUM_3]].high == ((0 as i32) as u32))) as i32;
// CHECK-NEXT:         [[NUM_3]].low = (0 as i32) as u32;
// CHECK-NEXT:         [[NUM_3]].high = (0 as i32) as u32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut orig: cpp_num = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut m: u32 = [[N_1]];
// CHECK-NEXT:         orig = [[NUM_3]];
// CHECK-NEXT:         if (m as u64) >= (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) {
// CHECK-NEXT:             let mut [[V_16:__v[0-9]+]]: u32 = m;
// CHECK-NEXT:             let mut [[V_17:__v[0-9]+]]: u32 = ([[V_16]] as u64)
// CHECK-NEXT:                 .wrapping_sub((4 as u64).wrapping_mul(((8 as i32) as i64) as u64))
// CHECK-NEXT:                 as u32;
// CHECK-NEXT:             m = [[V_17]];
// CHECK-NEXT:             [[NUM_3]].high = [[NUM_3]].low;
// CHECK-NEXT:             [[NUM_3]].low = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if m != (0 as u32) {
// CHECK-NEXT:             [[NUM_3]].high = [[NUM_3]].high << m
// CHECK-NEXT:                 | [[NUM_3]].low
// CHECK-NEXT:                     >> (4 as u64)
// CHECK-NEXT:                         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_sub(m as u64);
// CHECK-NEXT:             let mut [[V_18:__v[0-9]+]]: u32 = [[NUM_3]].low;
// CHECK-NEXT:             let mut [[V_19:__v[0-9]+]]: u32 = [[V_18]] << m;
// CHECK-NEXT:             [[NUM_3]].low = [[V_19]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[NUM_3]] = num_trim([[NUM_3]], [[PRECISION_3]]);
// CHECK-NEXT:         if [[NUM_3]].unsignedp != (0 as i32) {
// CHECK-NEXT:             [[NUM_3]].overflow = 0 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let mut maybe_orig: cpp_num = num_rshift([[NUM_3]], [[PRECISION_3]], [[N_1]]);
// CHECK-NEXT:             [[NUM_3]].overflow = !(orig.low == maybe_orig.low && orig.high == maybe_orig.high) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return [[NUM_3]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut precision: u32 = (64 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: u32 = (16 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut num: cpp_num = cpp_num {
// CHECK-NEXT:     high: (0 as i32) as u32,
// CHECK-NEXT:     low: (3 as i32) as u32,
// CHECK-NEXT:     unsignedp: 0 as i32,
// CHECK-NEXT:     overflow: 0 as i32,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut res: cpp_num = num_lshift(unsafe { num }, (64 as i32) as u32, unsafe { n });
// CHECK-NEXT:     if res.low != ((196608 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if res.high != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if res.overflow != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
