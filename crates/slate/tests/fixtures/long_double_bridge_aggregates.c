#include <stdio.h>

struct tagged {
  long double value;
  int tag;
};

struct big {
  double d[3];
  int k;
};

struct one {
  long double value;
};

long double weigh(struct big b, long double scale) {
  return (b.d[0] + b.d[1] + b.d[2] + b.k) * scale;
}

struct tagged make(long double value, int tag) {
  struct tagged t = {value / 3, tag + 1};
  return t;
}

struct big spread(long double base, struct big b) {
  struct big r = {{b.d[0] + (double)base, b.d[1] * 2, b.d[2] - 1}, b.k + 7};
  return r;
}

struct one halve(struct one o, struct tagged t) {
  struct one r = {o.value / 2 + t.value * t.tag};
  return r;
}

int main(void) {
  long double (*volatile weigh_ptr)(struct big, long double) = weigh;
  struct tagged (*volatile make_ptr)(long double, int) = make;
  struct big (*volatile spread_ptr)(long double, struct big) = spread;
  struct one (*volatile halve_ptr)(struct one, struct tagged) = halve;
  struct big b = {{1.5, 2.25, -0.75}, 4};
  struct tagged t = make_ptr(10.0L, 2);
  struct big s = spread_ptr(0.5L, b);
  struct one o = halve_ptr((struct one){9.0L}, t);
  printf("%.6Lf\n", weigh_ptr(b, 0.5L));
  printf("%.6Lf %d\n", t.value, t.tag);
  printf("%.3f %.3f %.3f %d\n", s.d[0], s.d[1], s.d[2], s.k);
  printf("%.6Lf\n", o.value);
  printf("%.6Lf\n", weigh(b, 2.0L) + make(1.0L, 0).value + halve(o, t).value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __slate_ldfp_long_double_bridge_aggregates_halve();
// CHECK-NEXT:     fn __slate_ldfp_long_double_bridge_aggregates_make();
// CHECK-NEXT:     fn __slate_ldfp_long_double_bridge_aggregates_spread();
// CHECK-NEXT:     fn __slate_ldfp_long_double_bridge_aggregates_weigh();
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
// CHECK-NEXT:     fn __slate_icall__rf80_m32a8_f80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: big,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_icall__rm32_f80_i32(_0: *const (), _1: LongDouble, _2: i32) -> tagged;
// CHECK-NEXT:     fn __slate_icall__rm32a8_f80_m32a8(_0: *const (), _1: LongDouble, _2: big) -> big;
// CHECK-NEXT:     fn __slate_icall__rs80_s80_m32(_0: *const (), _1: one, _2: tagged) -> one;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *const i8, _1: LongDouble) -> i32;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80_i32(_0: *const i8, _1: LongDouble, _2: i32) -> i32;
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
// CHECK-NEXT: struct tagged {
// CHECK-NEXT:     value: LongDouble,
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     d: [f64; 3],
// CHECK-NEXT:     k: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct one {
// CHECK-NEXT:     value: LongDouble,
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_long_double_bridge_aggregates_weigh__rf80_m32a8_f80"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.weigh")]
// CHECK-NEXT: extern "C-unwind" fn weigh(b: big, scale: LongDouble) -> LongDouble {
// CHECK-NEXT:     __slate_f80_from_f64(b.d[0] + b.d[1] + b.d[2] + (b.k as f64)) * scale
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_long_double_bridge_aggregates_make__rm32_f80_i32"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.make")]
// CHECK-NEXT: extern "C-unwind" fn make(value: LongDouble, tag: i32) -> tagged {
// CHECK-NEXT:     let t: tagged = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: tagged = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = value / __slate_f80_from_i32(3i32);
// CHECK-NEXT:         [[T_0]].tag = tag + 1;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     return t;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_long_double_bridge_aggregates_spread__rm32a8_f80_m32a8"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.spread")]
// CHECK-NEXT: extern "C-unwind" fn spread(base: LongDouble, b: big) -> big {
// CHECK-NEXT:     let r: big = {
// CHECK-NEXT:         let mut [[T_0]]: big = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].d = [
// CHECK-NEXT:             b.d[0] + __slate_f80_to_f64(base),
// CHECK-NEXT:             b.d[1] * (2i32 as f64),
// CHECK-NEXT:             b.d[2] - (1i32 as f64),
// CHECK-NEXT:         ];
// CHECK-NEXT:         [[T_0]].k = b.k + 7;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     return r;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_long_double_bridge_aggregates_halve__rs80_s80_m32"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.halve")]
// CHECK-NEXT: extern "C-unwind" fn halve(o: one, t: tagged) -> one {
// CHECK-NEXT:     let r: one = one {
// CHECK-NEXT:         value: o.value / __slate_f80_from_i32(2i32)
// CHECK-NEXT:             + t.value * __slate_f80_from_i32(t.tag),
// CHECK-NEXT:     };
// CHECK-NEXT:     return r;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: one = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: tagged = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: one = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let weigh_ptr: Option<unsafe extern "C-unwind" fn(big, LongDouble) -> LongDouble> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(big, LongDouble) -> LongDouble>,
// CHECK-NEXT:         >(__slate_ldfp_long_double_bridge_aggregates_weigh as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let make_ptr: Option<unsafe extern "C-unwind" fn(LongDouble, i32) -> tagged> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, i32) -> tagged>,
// CHECK-NEXT:         >(__slate_ldfp_long_double_bridge_aggregates_make as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let spread_ptr: Option<unsafe extern "C-unwind" fn(LongDouble, big) -> big> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, big) -> big>,
// CHECK-NEXT:         >(__slate_ldfp_long_double_bridge_aggregates_spread as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let halve_ptr: Option<unsafe extern "C-unwind" fn(one, tagged) -> one> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(one, tagged) -> one>,
// CHECK-NEXT:         >(__slate_ldfp_long_double_bridge_aggregates_halve as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: big = {
// CHECK-NEXT:         let mut [[T_0]]: big = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].d = [1.5f64, 2.25f64, -0.75f64];
// CHECK-NEXT:         [[T_0]].k = 4i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let t: tagged = unsafe {
// CHECK-NEXT:         __slate_icall__rm32_f80_i32(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, i32) -> tagged>,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(std::ptr::read_volatile(std::ptr::addr_of!(make_ptr)))
// CHECK-NEXT:             },
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]),
// CHECK-NEXT:             2i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let s: big = unsafe {
// CHECK-NEXT:         __slate_icall__rm32a8_f80_m32a8(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, big) -> big>,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(std::ptr::read_volatile(std::ptr::addr_of!(spread_ptr)))
// CHECK-NEXT:             },
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]),
// CHECK-NEXT:             b,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let o: one = unsafe {
// CHECK-NEXT:         __slate_icall__rs80_s80_m32(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(one, tagged) -> one>,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(std::ptr::read_volatile(std::ptr::addr_of!(halve_ptr)))
// CHECK-NEXT:             },
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = one {
// CHECK-NEXT:                     value: LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]),
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             },
// CHECK-NEXT:             t,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.6Lf\n".as_ptr(),
// CHECK-NEXT:             __slate_icall__rf80_m32a8_f80(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<
// CHECK-NEXT:                             unsafe extern "C-unwind" fn(big, LongDouble) -> LongDouble,
// CHECK-NEXT:                         >,
// CHECK-NEXT:                         *const (),
// CHECK-NEXT:                     >(std::ptr::read_volatile(std::ptr::addr_of!(weigh_ptr)))
// CHECK-NEXT:                 },
// CHECK-NEXT:                 b,
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { __slate_printf__ri32_pi8_f80_i32(c"%.6Lf %d\n".as_ptr(), t.value, t.tag) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%.3f %.3f %.3f %d\n".as_ptr(),
// CHECK-NEXT:             s.d[0],
// CHECK-NEXT:             s.d[1],
// CHECK-NEXT:             s.d[2],
// CHECK-NEXT:             s.k,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { __slate_printf__ri32_pi8_f80(c"%.6Lf\n".as_ptr(), o.value) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.6Lf\n".as_ptr(),
// CHECK-NEXT:             weigh(b, LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]))
// CHECK-NEXT:                 + (*{
// CHECK-NEXT:                     [[SLATE_COMPOUND_1]] = make(
// CHECK-NEXT:                         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:                         0i32,
// CHECK-NEXT:                     );
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .value
// CHECK-NEXT:                 + (*{
// CHECK-NEXT:                     [[SLATE_COMPOUND_2]] = halve(o, t);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .value,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
