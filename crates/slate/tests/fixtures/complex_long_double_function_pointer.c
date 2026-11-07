#include <complex.h>
#include <stdio.h>

typedef long double _Complex (*cunary)(long double _Complex);
typedef long double (*cpart)(long double _Complex);

static long double _Complex twice(long double _Complex z) { return z * 2; }
long double _Complex swap(long double _Complex z) { return __imag__ z + __real__ z * 1.0iL; }
static long double imag_part(long double _Complex z) { return __imag__ z; }

static long double _Complex apply(cunary f, long double _Complex z) { return f(z); }

int main(void) {
    cunary table[] = {twice, swap, conjl, twice};
    long double _Complex z = 1.5L + 0.25iL;
    for (int i = 0; i < 4; i++) {
        z = apply(table[i], z);
        printf("%d %.6Lf %.6Lf\n", i, creall(z), cimagl(z));
    }
    cpart parts[] = {imag_part, creall, cimagl};
    for (int i = 0; i < 3; i++)
        printf("p%d %.6Lf\n", i, parts[i](3.0L - 4.0iL));
    printf("eq %d %d\n", table[0] == table[3], table[0] == (cunary)twice);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "cimagl"]
// CHECK-NEXT:     fn __slate_ldfp_complex_long_double_function_pointer_cimagl();
// CHECK-NEXT:     #[link_name = "conjl"]
// CHECK-NEXT:     fn __slate_ldfp_complex_long_double_function_pointer_conjl();
// CHECK-NEXT:     #[link_name = "creall"]
// CHECK-NEXT:     fn __slate_ldfp_complex_long_double_function_pointer_creall();
// CHECK-NEXT:     fn __slate_ldfp_complex_long_double_function_pointer_imag_part();
// CHECK-NEXT:     fn __slate_ldfp_complex_long_double_function_pointer_swap();
// CHECK-NEXT:     fn __slate_ldfp_complex_long_double_function_pointer_twice();
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
// CHECK-NEXT:     fn __slate_cimagl__rf80_cf80(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// CHECK-NEXT:     fn __slate_creall__rf80_cf80(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// CHECK-NEXT:     fn __slate_icall__rcf80_cf80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     fn __slate_icall__rf80_cf80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_i32_f80(_0: *const i8, _1: i32, _2: LongDouble) -> i32;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_i32_f80_f80(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> i32;
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_complex_long_double_function_pointer_twice__rcf80_cf80"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.twice")]
// CHECK-NEXT: extern "C-unwind" fn twice(
// CHECK-NEXT:     z: num_complex::Complex<LongDouble>,
// CHECK-NEXT: ) -> num_complex::Complex<LongDouble> {
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = z;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = __slate_f80_from_i32(2i32);
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re * [[T_1]],
// CHECK-NEXT:             im: [[T_0]].im * [[T_1]],
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_complex_long_double_function_pointer_swap__rcf80_cf80"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.swap")]
// CHECK-NEXT: extern "C-unwind" fn swap(
// CHECK-NEXT:     z: num_complex::Complex<LongDouble>,
// CHECK-NEXT: ) -> num_complex::Complex<LongDouble> {
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = z.im;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_0]] = z.re;
// CHECK-NEXT:             let [[T_1]] = num_complex::Complex {
// CHECK-NEXT:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// CHECK-NEXT:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:             };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_0]] * [[T_1]].re,
// CHECK-NEXT:                 im: [[T_0]] * [[T_1]].im,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_2]] + [[T_3]].re,
// CHECK-NEXT:             im: [[T_3]].im,
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ld____slate_ldfp_complex_long_double_function_pointer_imag_part__rf80_cf80"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.imag_part")]
// CHECK-NEXT: extern "C-unwind" fn imag_part(z: num_complex::Complex<LongDouble>) -> LongDouble {
// CHECK-NEXT:     z.im
// CHECK-NEXT: }
// CHECK-NEXT: fn apply(
// CHECK-NEXT:     f: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(
// CHECK-NEXT:             num_complex::Complex<LongDouble>,
// CHECK-NEXT:         ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:     >,
// CHECK-NEXT:     z: num_complex::Complex<LongDouble>,
// CHECK-NEXT: ) -> num_complex::Complex<LongDouble> {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_icall__rcf80_cf80(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<
// CHECK-NEXT:                         unsafe extern "C-unwind" fn(
// CHECK-NEXT:                             num_complex::Complex<LongDouble>,
// CHECK-NEXT:                         ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     >,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(f)
// CHECK-NEXT:             },
// CHECK-NEXT:             z,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let table: __SlateAlign16<
// CHECK-NEXT:         [Option<
// CHECK-NEXT:             unsafe extern "C-unwind" fn(
// CHECK-NEXT:                 num_complex::Complex<LongDouble>,
// CHECK-NEXT:             ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:         >; 4],
// CHECK-NEXT:     > = __SlateAlign16([
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_twice as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_swap as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_conjl as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> num_complex::Complex<LongDouble>,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_twice as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut z: num_complex::Complex<LongDouble> = {
// CHECK-NEXT:         let [[T_0]] = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// CHECK-NEXT:         let [[T_1]] = num_complex::Complex {
// CHECK-NEXT:             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// CHECK-NEXT:             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]),
// CHECK-NEXT:         };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]] + [[T_1]].re,
// CHECK-NEXT:             im: [[T_1]].im,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         z = apply(table.0[i as usize], z);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_printf__ri32_pi8_i32_f80_f80(
// CHECK-NEXT:                 c"%d %.6Lf %.6Lf\n".as_ptr(),
// CHECK-NEXT:                 i,
// CHECK-NEXT:                 __slate_creall__rf80_cf80(z),
// CHECK-NEXT:                 __slate_cimagl__rf80_cf80(z),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let parts: __SlateAlign16<
// CHECK-NEXT:         [Option<
// CHECK-NEXT:             unsafe extern "C-unwind" fn(num_complex::Complex<LongDouble>) -> LongDouble,
// CHECK-NEXT:         >; 3],
// CHECK-NEXT:     > = __SlateAlign16([
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> LongDouble,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_imag_part as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> LongDouble,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_creall as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<
// CHECK-NEXT:                     unsafe extern "C-unwind" fn(
// CHECK-NEXT:                         num_complex::Complex<LongDouble>,
// CHECK-NEXT:                     ) -> LongDouble,
// CHECK-NEXT:                 >,
// CHECK-NEXT:             >(__slate_ldfp_complex_long_double_function_pointer_cimagl as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..3i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_printf__ri32_pi8_i32_f80(
// CHECK-NEXT:                 c"p%d %.6Lf\n".as_ptr(),
// CHECK-NEXT:                 i,
// CHECK-NEXT:                 __slate_icall__rf80_cf80(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             Option<
// CHECK-NEXT:                                 unsafe extern "C-unwind" fn(
// CHECK-NEXT:                                     num_complex::Complex<LongDouble>,
// CHECK-NEXT:                                 ) -> LongDouble,
// CHECK-NEXT:                             >,
// CHECK-NEXT:                             *const (),
// CHECK-NEXT:                         >(parts.0[i as usize])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_2]] = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// CHECK-NEXT:                         let [[T_3]] = num_complex::Complex {
// CHECK-NEXT:                             re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// CHECK-NEXT:                             im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// CHECK-NEXT:                         };
// CHECK-NEXT:                         num_complex::Complex {
// CHECK-NEXT:                             re: [[T_2]] - [[T_3]].re,
// CHECK-NEXT:                             im: -[[T_3]].im,
// CHECK-NEXT:                         }
// CHECK-NEXT:                     },
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "eq {} {}", (table.0[0] == table.0[3]) as i32, (table.0[0] == unsafe {
// CHECK-NEXT:         std::mem::transmute::<* const (), Option < unsafe extern "C-unwind" fn
// CHECK-NEXT:         (num_complex::Complex < LongDouble >) -> num_complex::Complex < LongDouble >>>
// CHECK-NEXT:         (__slate_ldfp_complex_long_double_function_pointer_twice as * const ()) }) as i32
// CHECK-NEXT:     );
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
