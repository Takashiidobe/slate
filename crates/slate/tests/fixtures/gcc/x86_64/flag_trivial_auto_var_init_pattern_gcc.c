/* { dg-options "-ftrivial-auto-var-init=pattern" } */
int printf(const char *, ...);

struct mixed {
  int i;
  float f;
  char c;
  double d;
};

struct flags {
  _Bool b[2];
  char c;
};

struct bits {
  char pre;
  unsigned a : 3;
  unsigned b : 13;
  unsigned c : 2;
  char post;
};

union either {
  char c;
  long l;
};

struct narrow {
  unsigned a : 3;
};

struct seven {
  unsigned a : 3;
  unsigned b : 4;
};

struct nine {
  unsigned a : 3;
  unsigned b : 3;
  unsigned c : 3;
};

struct split {
  unsigned a : 5;
  char mid;
  unsigned b : 5;
};

struct wide {
  unsigned long a : 40;
  unsigned b : 3;
};

union holes {
  char c[5];
  int i;
};

union bits_only {
  unsigned a : 9;
};

union nested_bits {
  struct {
    char c;
    unsigned a : 3;
  } s;
};

union padded_member {
  struct {
    char c;
    int i;
  } s;
  float f;
};

struct tail {
  char c;
  long double x;
};

struct pair {
  int a;
  float b;
};

static unsigned hash(const void *p, unsigned size) {
  const unsigned char *bytes = p;
  unsigned h = 2166136261u;
  for (unsigned i = 0; i < size; i++)
    h = (h ^ bytes[i]) * 16777619u;
  return h;
}

static void dump(const char *name, const void *p, unsigned size) {
  const unsigned char *bytes = p;
  printf("%s:", name);
  for (unsigned i = 0; i < size; i++)
    printf(" %02x", bytes[i]);
  printf("\n");
}

int main(void) {
  int i;
  float f;
  double d;
  long double ld;
  void *p;
  _Bool lone;
  _Complex float cf;
  int arr[3];
  struct mixed mix;
  struct flags fl;
  struct bits bf;
  union either un;
  struct narrow nr;
  struct seven sv;
  struct nine nn;
  struct split sp;
  struct wide wd;
  union holes holes;
  union bits_only bits_only;
  union nested_bits nested_bits;
  union padded_member padded_member;
  struct tail tail;
  _Complex long double cld;
  unsigned _BitInt(7) b7;
  _BitInt(65) b65;
  struct pair pairs[600];
  dump("int", &i, sizeof i);
  dump("float", &f, sizeof f);
  dump("double", &d, sizeof d);
  dump("long double", &ld, 10);
  dump("pointer", &p, sizeof p);
  dump("lone bool", &lone, sizeof lone);
  dump("complex", &cf, sizeof cf);
  dump("array", arr, sizeof arr);
  dump("mixed.i", &mix.i, sizeof mix.i);
  dump("mixed.f", &mix.f, sizeof mix.f);
  dump("mixed.c", &mix.c, sizeof mix.c);
  dump("mixed.d", &mix.d, sizeof mix.d);
  dump("flags.b", fl.b, sizeof fl.b);
  dump("flags.c", &fl.c, sizeof fl.c);
  dump("bits.pre", &bf.pre, sizeof bf.pre);
  dump("bits.post", &bf.post, sizeof bf.post);
  dump("bits", &bf, 4);
  dump("union", &un, sizeof un);
  dump("narrow", &nr, 1);
  dump("seven", &sv, 1);
  dump("nine", &nn, 2);
  dump("split", &sp, 3);
  dump("wide", &wd, 6);
  dump("holes", &holes, sizeof holes);
  dump("bits_only", &bits_only, sizeof bits_only);
  dump("nested_bits", &nested_bits, sizeof nested_bits);
  dump("padded_member", &padded_member, sizeof padded_member);
  dump("tail", &tail, sizeof tail);
  dump("complex long double", &cld, sizeof cld);
  dump("bitint7", &b7, sizeof b7);
  dump("bitint65", &b65, sizeof b65);
  printf("pairs: %08x\n", hash(pairs, sizeof pairs));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
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
// CHECK-NEXT:     safe fn __slate_cf80_add(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_sub(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_mul(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_mul_basic(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div_basic(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div_improved(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_neg(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
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
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-ftrivial-auto-var-init=pattern" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mixed {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     f: f32,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct flags {
// CHECK-NEXT:     b: [bool; 2],
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     pre: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT:     post: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union either {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     l: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct narrow {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct seven {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct nine {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits6U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct split {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits7U0,
// CHECK-NEXT:     mid: i8,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits7U1,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union holes {
// CHECK-NEXT:     c: [i8; 5],
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union bits_only {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits10U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union nested_bits {
// CHECK-NEXT:     s: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits12U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union padded_member {
// CHECK-NEXT:     s: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tail {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     x: LongDouble,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits6U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits7U0 {
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(3, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits7U1 {
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(3, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield([u8; 6], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(40)]
// CHECK-NEXT:         pub a: u64,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits10U0 {
// CHECK-NEXT:         #[bits(9)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits12U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn hash(p: *const (), size: u32) -> u32 {
// CHECK-NEXT:     let bytes: *const u8 = p as *const u8;
// CHECK-NEXT:     let mut h: u32 = 2166136261;
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         h = (h ^ ((((unsafe { *bytes.add(i as usize) }) as u32) as i32) as u32))
// CHECK-NEXT:             .wrapping_mul(16777619);
// CHECK-NEXT:     }
// CHECK-NEXT:     h
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dump(name: *const i8, p: *const (), size: u32) {
// CHECK-NEXT:     let bytes: *const u8 = p as *const u8;
// CHECK-NEXT:     unsafe { printf(c"%s:".as_ptr(), name) };
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         unsafe { printf(c" %02x".as_ptr(), (*bytes.add(i as usize) as u32) as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { printf(c"\n".as_ptr()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -16843010i32;
// CHECK-NEXT:     let mut f: f32 = f32::from_bits(4278124286u32);
// CHECK-NEXT:     let mut d: f64 = f64::from_bits(18374403900871474942u64);
// CHECK-NEXT:     let mut ld: LongDouble = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(ld) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 16usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 10usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut p: *mut () = 18374403900871474942usize as *mut ();
// CHECK-NEXT:     let mut lone: bool = false;
// CHECK-NEXT:     let mut cf: num_complex::Complex<f32> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(cf) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut arr: [i32; 3] = [-16843010i32; 3];
// CHECK-NEXT:     let mut mix: mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(mix) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 24usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 9usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(16usize), 254u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut fl: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(fl) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 3usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut bf: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(bf) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 8usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 3usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(3usize), 2u8, 1usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(4usize), 254u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut un: either = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(un) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut nr: narrow = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(nr) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 6u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sv: seven = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(sv) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 126u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut nn: nine = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(nn) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sp: split = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(sp) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 30u8, 1usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(1usize), 254u8, 1usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(2usize), 30u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut wd: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(wd) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 8usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 5usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(5usize), 6u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut holes: holes = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(holes) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 8usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 5usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut bits_only: bits_only = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(bits_only) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 2usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut nested_bits: nested_bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(nested_bits) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 1usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(1usize), 6u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut padded_member: padded_member = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(padded_member) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut tail: tail = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(tail) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 32usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 1usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(16usize), 254u8, 10usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut cld: num_complex::Complex<LongDouble> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(cld) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 32usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b7: u8 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(b7) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 126u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b65: bitint::BInt<65, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(b65) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 16usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut pairs: __SlateAlign16<[pair; 600]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(pairs.0) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 254u8, 4800usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"int".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(i) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"float".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(f) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"double".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(d) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"long double".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(ld) as *const (),
// CHECK-NEXT:         10u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"pointer".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(p) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"lone bool".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(lone) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"complex".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(cf) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(c"array".as_ptr(), arr.as_mut_ptr() as *const (), 12u32);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"mixed.i".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(mix.i) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"mixed.f".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(mix.f) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"mixed.c".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(mix.c) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"mixed.d".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(mix.d) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(c"flags.b".as_ptr(), fl.b.as_mut_ptr() as *const (), 2u32);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"flags.c".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(fl.c) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits.pre".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(bf.pre) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits.post".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(bf.post) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(bf) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"union".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(un) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"narrow".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(nr) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"seven".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(sv) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"nine".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(nn) as *const (),
// CHECK-NEXT:         2u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"split".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(sp) as *const (),
// CHECK-NEXT:         3u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"wide".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(wd) as *const (),
// CHECK-NEXT:         6u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"holes".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(holes) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits_only".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(bits_only) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"nested_bits".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(nested_bits) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"padded_member".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(padded_member) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"tail".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(tail) as *const (),
// CHECK-NEXT:         32u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"complex long double".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(cld) as *const (),
// CHECK-NEXT:         32u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bitint7".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(b7) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bitint65".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(b65) as *const (),
// CHECK-NEXT:         16u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"pairs: %08x\n".as_ptr(),
// CHECK-NEXT:             hash(pairs.0.as_mut_ptr() as *const (), 4800u32),
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
