/* { dg-options "-ftrivial-auto-var-init=pattern" } */
int printf(const char *, ...);

struct padded {
  char c;
  int i;
};

struct mixed {
  int i;
  float f;
  char c;
  double d;
};

struct floats {
  float f[2];
  _Bool b;
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

union int_float {
  int i;
  float f;
};

union float_int {
  float f;
  int i;
};

union double_long {
  double d;
  long long l;
};

union long_double {
  long long l;
  double d;
};

union floats_double {
  float f[3];
  double d;
};

union bits_float {
  unsigned a : 3;
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
  struct padded pad;
  struct padded pads[2];
  struct mixed mix;
  struct floats fl;
  struct bits bf;
  union either un;
  union int_float int_float;
  union float_int float_int;
  union double_long double_long;
  union long_double long_double;
  union floats_double floats_double;
  union bits_float bits_float;
  struct tail tail;
  struct tail tails[2];
  _Complex long double cld;
  struct pair pairs[600];
  dump("int", &i, sizeof i);
  dump("float", &f, sizeof f);
  dump("double", &d, sizeof d);
  dump("long double", &ld, 10);
  dump("pointer", &p, sizeof p);
  dump("bool", &lone, sizeof lone);
  dump("complex", &cf, sizeof cf);
  dump("array", arr, sizeof arr);
  dump("padded", &pad, sizeof pad);
  dump("padded[]", pads, sizeof pads);
  dump("mixed", &mix, sizeof mix);
  dump("floats", &fl, sizeof fl);
  dump("bits", &bf, sizeof bf);
  dump("union", &un, sizeof un);
  dump("int_float", &int_float, sizeof int_float);
  dump("float_int", &float_int, sizeof float_int);
  dump("double_long", &double_long, sizeof double_long);
  dump("long_double", &long_double, sizeof long_double);
  dump("floats_double", &floats_double, sizeof floats_double);
  dump("bits_float", &bits_float, sizeof bits_float);
  dump("tail", &tail, sizeof tail);
  dump("tails", tails, sizeof tails);
  dump("complex long double", &cld, sizeof cld);
  printf("pairs: %08x\n", hash(pairs, sizeof pairs));
  dump("pairs[599]", &pairs[599], sizeof pairs[599]);
  const unsigned char *raw = (const unsigned char *)&pad;
  printf("padding byte: %02x\n", raw[1]);
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
// CHECK-NEXT: struct padded {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-NEXT: struct floats {
// CHECK-NEXT:     f: [f32; 2],
// CHECK-NEXT:     b: bool,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     pre: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union int_float {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union float_int {
// CHECK-NEXT:     f: f32,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union double_long {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     l: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union long_double {
// CHECK-NEXT:     l: i64,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union floats_double {
// CHECK-NEXT:     f: [f32; 3],
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union bits_float {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits10U0,
// CHECK-NEXT:     f: f32,
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
// CHECK-NEXT:     pub struct __SlateBits3U0 {
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
// CHECK-NEXT:     pub struct __SlateBits10U0 {
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
// CHECK-NEXT:     let mut i: i32 = -1431655766i32;
// CHECK-NEXT:     let mut f: f32 = f32::from_bits(4294967295u32);
// CHECK-NEXT:     let mut d: f64 = f64::from_bits(18446744073709551615u64);
// CHECK-NEXT:     let mut ld: LongDouble = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(ld) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 16usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 10usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(10usize), 0u8, 6usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut p: *mut () = 12297829382473034410usize as *mut ();
// CHECK-NEXT:     let mut lone: bool = false;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(lone) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 1usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut cf: num_complex::Complex<f32> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(cf) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut arr: [i32; 3] = [-1431655766i32; 3];
// CHECK-NEXT:     let mut pad: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(pad) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut pads: __SlateAlign16<[padded; 2]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(pads.0) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 16usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut mix: mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(mix) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 24usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(4usize), 255u8, 4usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(16usize), 255u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut fl: floats = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(fl) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 12usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut bf: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(bf) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut un: either = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(un) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut int_float: int_float = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(int_float) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 4usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut float_int: float_int = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(float_int) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 4usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut double_long: double_long = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(double_long) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut long_double: long_double = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(long_double) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut floats_double: floats_double = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(floats_double) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 16usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 8usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut bits_float: bits_float = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(bits_float) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 4usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut tail: tail = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(tail) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 32usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(16usize), 255u8, 10usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(26usize), 0u8, 6usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut tails: [tail; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(tails) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 64usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(16usize), 255u8, 10usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(26usize), 0u8, 6usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(48usize), 255u8, 10usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(58usize), 0u8, 6usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut cld: num_complex::Complex<LongDouble> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(cld) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 32usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 255u8, 10usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(10usize), 0u8, 6usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(16usize), 255u8, 10usize);
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(26usize), 0u8, 6usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut pairs: __SlateAlign16<[pair; 600]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(pairs.0) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 170u8, 4800usize);
// CHECK-NEXT:         for __slate_auto_index0 in 0usize..600usize {
// CHECK-NEXT:             let __slate_auto_bytes0 = __slate_auto_bytes.add(0usize + __slate_auto_index0 * 8usize);
// CHECK-NEXT:             std::ptr::write_bytes(__slate_auto_bytes0.add(4usize), 255u8, 4usize);
// CHECK-NEXT:         }
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
// CHECK-NEXT:         c"bool".as_ptr(),
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
// CHECK-NEXT:         c"padded".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(pad) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"padded[]".as_ptr(),
// CHECK-NEXT:         pads.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         16u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"mixed".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(mix) as *const (),
// CHECK-NEXT:         24u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"floats".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(fl) as *const (),
// CHECK-NEXT:         12u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(bf) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"union".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(un) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"int_float".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(int_float) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"float_int".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(float_int) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"double_long".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(double_long) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"long_double".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(long_double) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"floats_double".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(floats_double) as *const (),
// CHECK-NEXT:         16u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits_float".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(bits_float) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"tail".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(tail) as *const (),
// CHECK-NEXT:         32u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(c"tails".as_ptr(), tails.as_mut_ptr() as *const (), 64u32);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"complex long double".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(cld) as *const (),
// CHECK-NEXT:         32u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"pairs: %08x\n".as_ptr(),
// CHECK-NEXT:             hash(pairs.0.as_mut_ptr() as *const (), 4800u32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"pairs[599]".as_ptr(),
// CHECK-NEXT:         (unsafe { (pairs.0.as_mut_ptr() as *mut pair).add(599) }) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let raw: *const u8 = std::ptr::addr_of_mut!(pad) as *const u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"padding byte: %02x\n".as_ptr(),
// CHECK-NEXT:             (*raw.add(1) as u32) as i32,
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
