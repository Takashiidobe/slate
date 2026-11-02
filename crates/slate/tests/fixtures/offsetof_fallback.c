#include <stddef.h>
#include <stdio.h>

struct Inner {
  char tag;
  int  value;
};

union Either {
  char         small;
  struct Inner inner;
};

struct Outer {
  char         first;
  struct Inner inner;
  union Either either;
  int          type;
  int          list[4];
  struct Inner items[2];
};

struct Packed {
  char tag;
  int  value;
} __attribute__((packed));

struct Spaced {
  char tag;
  int  value __attribute__((aligned(32)));
  char last;
};

struct Bits {
  unsigned low : 3;
  unsigned high : 5;
  int      after;
};

struct Holder {
  long double wide;
  char        tag;
};

struct Flexible {
  int  count;
  char data[];
};

struct Anonymous {
  char tag;
  struct {
    short left;
    int   right;
  };
  union {
    char   small;
    double large;
  };
};

struct Raised {
  char tag;
  int  value;
} __attribute__((aligned(16)));

struct Erasing {
  char          tag;
  struct Raised raised;
} __attribute__((packed));

static char table[offsetof(struct Inner, value)];
static const unsigned long global_offset = offsetof(struct Outer, type) * 2;

enum { TYPE_OFFSET = offsetof(struct Outer, type) };

_Static_assert(offsetof(struct Inner, tag) == 0, "first member");

static int classify(unsigned long offset) {
  switch (offset) {
  case offsetof(struct Inner, tag):
    return 1;
  case offsetof(struct Inner, value):
    return 2;
  default:
    return 0;
  }
}

static void report_folded(int index) {
  printf("%d\n", (int)offsetof(struct Outer, list[2]));
  printf("%d\n", (int)offsetof(struct Outer, items[1].value));
  printf("%d\n", (int)offsetof(struct Outer, list[index]));
  printf("%d\n", (int)offsetof(struct Holder, tag));
  printf("%d\n", (int)offsetof(struct Flexible, data));
  printf("%d\n", (int)offsetof(struct Anonymous, right));
  printf("%d\n", (int)offsetof(struct Anonymous, large));
  printf("%d\n", (int)offsetof(struct Erasing, raised.value));
}

static void report_kept(void) {
  printf("%d\n", (int)offsetof(struct Inner, value));
  printf("%d\n", (int)offsetof(struct Outer, inner));
  printf("%d\n", (int)offsetof(struct Outer, inner.value));
  printf("%d\n", (int)offsetof(struct Outer, either.inner.value));
  printf("%d\n", (int)offsetof(struct Outer, type));
  printf("%d\n", (int)offsetof(struct Outer, list));
  printf("%d\n", (int)offsetof(union Either, small));
  printf("%d\n", (int)offsetof(union Either, inner.value));
  printf("%d\n", (int)offsetof(struct Packed, value));
  printf("%d\n", (int)offsetof(struct Spaced, value));
  printf("%d\n", (int)offsetof(struct Spaced, last));
  printf("%d\n", (int)offsetof(struct Bits, after));
  printf("%d\n", (int)offsetof(struct Anonymous, tag));
  printf("%d\n", (int)offsetof(struct Erasing, raised));
}

static int value_of(int *value) {
  struct Inner *owner =
      (struct Inner *)((char *)value - offsetof(struct Inner, value));
  return owner->tag + owner->value;
}

int main(void) {
  struct Inner inner = {1, 41};
  report_folded(3);
  report_kept();
  printf("%d\n", (int)sizeof(table));
  printf("%lu\n", global_offset);
  printf("%d\n", (int)TYPE_OFFSET);
  printf("%d\n", classify(offsetof(struct Inner, tag)));
  printf("%d\n", classify(offsetof(struct Inner, value)));
  printf("%d\n", classify(offsetof(struct Outer, type)));
  printf("%d\n", value_of(&inner.value));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Inner {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Either {
// CHECK-NEXT:     small: i8,
// CHECK-NEXT:     inner: Inner,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Outer {
// CHECK-NEXT:     first: i8,
// CHECK-NEXT:     inner: Inner,
// CHECK-NEXT:     either: Either,
// CHECK-NEXT:     r#type: i32,
// CHECK-NEXT:     list: [i32; 4],
// CHECK-NEXT:     items: [Inner; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Packed {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Spaced {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 31],
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     last: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT:     after: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Holder {
// CHECK-NEXT:     wide: LongDouble,
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Anonymous {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_offsetof_fallback_c_52,
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: __SlateRecord_offsetof_fallback_c_56,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_offsetof_fallback_c_52 {
// CHECK-NEXT:     left: i16,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_offsetof_fallback_c_56 {
// CHECK-NEXT:     small: i8,
// CHECK-NEXT:     large: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Raised {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Erasing {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     raised: std::mem::MaybeUninit<[u8; 16]>,
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
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub low: u32,
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub high: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut table: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut global_offset: u64 = (std::mem::offset_of!(Outer, r#type) as u64)
// CHECK-NEXT:     .wrapping_mul(2u64);
// CHECK-NEXT: fn classify(offset: u64) -> i32 {
// CHECK-NEXT:     match offset {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn report_folded(index: i32) {
// CHECK-NEXT:     println!("{}", 32i32);
// CHECK-NEXT:     println!("{}", 52i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (24u64.wrapping_add(((index as i64) as u64).wrapping_mul(4)) as u32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Holder, tag) as i32);
// CHECK-NEXT:     println!("{}", 4i32);
// CHECK-NEXT:     println!("{}", 8i32);
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", 5i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn report_kept() {
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Inner, value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, inner) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, inner.value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, either.inner.value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, r#type) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, list) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Either, small) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Either, inner.value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Packed, value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Spaced, value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Spaced, last) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Bits, after) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Anonymous, tag) as i32);
// CHECK-NEXT:     println!("{}", 1i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn value_of(value: *mut i32) -> i32 {
// CHECK-NEXT:     let owner: *mut Inner = (unsafe {
// CHECK-NEXT:         (value as *mut i8).offset(-(std::mem::offset_of!(Inner, value) as isize))
// CHECK-NEXT:     }) as *mut Inner;
// CHECK-NEXT:     ((unsafe { (*owner).tag }) as i32) + unsafe { (*owner).value }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut inner: Inner = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Inner = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = 1i8;
// CHECK-NEXT:         [[T_0]].value = 41i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     report_folded(3i32);
// CHECK-NEXT:     report_kept();
// CHECK-NEXT:     println!("{}", std::mem::size_of::< [i8; 4] > () as i32);
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), global_offset) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", 20i32);
// CHECK-NEXT:     println!("{}", classify(std::mem::offset_of!(Inner, tag) as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::offset_of!(Inner, value) as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::offset_of!(Outer, r#type) as u64));
// CHECK-NEXT:     println!("{}", value_of(std::ptr::addr_of_mut!(inner.value)));
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
