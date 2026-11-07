#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static long double twice_sum(long double scale, int count, ...) {
    va_list ap;
    long double total = 0;
    for (int pass = 0; pass < 2; pass++) {
        va_start(ap, count);
        for (int i = 0; i < count; i++)
            total += va_arg(ap, long double);
        va_end(ap);
    }
    return total * scale;
}

static long double mixed(long double base, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    for (const char *f = format; *f; f++) {
        if (*f == 'i')
            base += va_arg(ap, int);
        else if (*f == 'd')
            base -= va_arg(ap, double);
        else
            base *= va_arg(ap, long double);
    }
    va_end(ap);
    return base;
}

static void report(int count, ...) {
    va_list ap;
    va_start(ap, count);
    for (int i = 0; i < count; i++)
        printf("%.21Lg\n", va_arg(ap, long double));
    va_end(ap);
}

static void dump(const char *name, long double value) {
    unsigned char bytes[sizeof value];
    memcpy(bytes, &value, sizeof value);
    printf("%s ", name);
    for (int i = 10; i-- > 0;)
        printf("%02x", bytes[i]);
    printf("\n");
}

int main(void) {
    long double third = 1.0L / 3;
    long double (*sum)(long double, int, ...) = twice_sum;
    long double (*mix)(long double, const char *, ...) = mixed;
    void (*print)(int, ...) = report;
    dump("direct", twice_sum(1.5L, 2, 1.25L, third));
    dump("pointer", sum(1.5L, 2, 1.25L, third));
    dump("none", sum(third, 0));
    dump("mixed", mix(third, "idLi", 7, 0.5, 3.0L, -2));
    print(2, third, sum(2.0L, 1, third));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __slate_ldfp_long_double_variadic_definition_address_mixed();
// CHECK-NEXT:     fn __slate_ldfp_long_double_variadic_definition_address_twice_sum();
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
// CHECK-NEXT:     fn __slate_icall__rf80_f80_i32__(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: i32,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_icall__rf80_f80_i32__f80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: i32,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_icall__rf80_f80_i32__f80_f80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: i32,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:         _4: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_icall__rf80_f80_pi8__i32_f64_f80_i32(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: *const i8,
// CHECK-NEXT:         _3: i32,
// CHECK-NEXT:         _4: f64,
// CHECK-NEXT:         _5: LongDouble,
// CHECK-NEXT:         _6: i32,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_icall__rv_i32__f80_f80(
// CHECK-NEXT:         _0: *const (),
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     );
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *const i8, _1: LongDouble) -> i32;
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
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ldv____slate_ldfp_long_double_variadic_definition_address_twice_sum__rf80_f80_i32"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.twice_sum")]
// CHECK-NEXT: unsafe extern "C-unwind" fn twice_sum(
// CHECK-NEXT:     scale: LongDouble,
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     mut __va_args: core::ffi::VaList<'static>,
// CHECK-NEXT: ) -> LongDouble {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: LongDouble = __slate_f80_from_i32(0i32);
// CHECK-NEXT:     for pass in 0i32..2i32 {
// CHECK-NEXT:         ap = unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::ffi::VaList<'_>,
// CHECK-NEXT:                 core::ffi::VaList<'static>,
// CHECK-NEXT:             >(__va_args.clone())
// CHECK-NEXT:         };
// CHECK-NEXT:         for i in 0..count {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: LongDouble = unsafe {
// CHECK-NEXT:                 __slate_f80_va_arg(std::ptr::addr_of_mut!(ap))
// CHECK-NEXT:             };
// CHECK-NEXT:             total += [[V_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total * scale
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(
// CHECK-NEXT:     export_name = "__slate_ldv____slate_ldfp_long_double_variadic_definition_address_mixed__rf80_f80_pi8"
// CHECK-NEXT: )]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.mixed")]
// CHECK-NEXT: unsafe extern "C-unwind" fn mixed(
// CHECK-NEXT:     mut base: LongDouble,
// CHECK-NEXT:     mut f: *const i8,
// CHECK-NEXT:     mut __va_args: core::ffi::VaList<'static>,
// CHECK-NEXT: ) -> LongDouble {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     while (unsafe { *f }) != 0 {
// CHECK-NEXT:         if ((unsafe { *f }) as i32) == 105 {
// CHECK-NEXT:             let [[V_0]]: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: LongDouble = base;
// CHECK-NEXT:             base = [[V_1]] + __slate_f80_from_i32([[V_0]]);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if ((unsafe { *f }) as i32) == 100 {
// CHECK-NEXT:                 let [[V_2:__v[0-9]+]]: f64 = unsafe { ap.next_arg::<f64>() };
// CHECK-NEXT:                 let [[V_3:__v[0-9]+]]: LongDouble = base;
// CHECK-NEXT:                 base = [[V_3]] - __slate_f80_from_f64([[V_2]]);
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let [[V_4:__v[0-9]+]]: LongDouble = unsafe {
// CHECK-NEXT:                     __slate_f80_va_arg(std::ptr::addr_of_mut!(ap))
// CHECK-NEXT:                 };
// CHECK-NEXT:                 base *= [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: *const i8 = f;
// CHECK-NEXT:         f = unsafe { [[V_5]].add(1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     base
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.report")]
// CHECK-NEXT: unsafe extern "C-unwind" fn report(count: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:                 c"%.21Lg\n".as_ptr(),
// CHECK-NEXT:                 __slate_f80_va_arg(std::ptr::addr_of_mut!(ap)),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn dump(name: *const i8, mut value: LongDouble) {
// CHECK-NEXT:     let mut bytes: __SlateAlign16<[u8; 16]> = __SlateAlign16([0u8; 16]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(value) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((bytes.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<LongDouble>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s ".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut i: i32 = 10;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_0]]: i32 = i;
// CHECK-NEXT:         i = [[V_0]] - 1;
// CHECK-NEXT:         if [[V_0]] <= 0 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%02x".as_ptr(), bytes.0[i as usize] as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let third: LongDouble = LongDouble([
// CHECK-NEXT:         171, 170, 170, 170, 170, 170, 170, 170, 253, 63,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let sum: Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, i32, ...) -> LongDouble>,
// CHECK-NEXT:         >(__slate_ldfp_long_double_variadic_definition_address_twice_sum as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mix: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(LongDouble, *const i8, ...) -> LongDouble,
// CHECK-NEXT:     > = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(LongDouble, *const i8, ...) -> LongDouble>,
// CHECK-NEXT:         >(__slate_ldfp_long_double_variadic_definition_address_mixed as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let print: Option<unsafe extern "C-unwind" fn(i32, ...)> = Some(report);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"direct".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_icall__rf80_f80_i32__f80_f80(
// CHECK-NEXT:                 __slate_ldfp_long_double_variadic_definition_address_twice_sum
// CHECK-NEXT:                     as *const (),
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]),
// CHECK-NEXT:                 2i32,
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 63]),
// CHECK-NEXT:                 third,
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"pointer".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_icall__rf80_f80_i32__f80_f80(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<
// CHECK-NEXT:                             unsafe extern "C-unwind" fn(
// CHECK-NEXT:                                 LongDouble,
// CHECK-NEXT:                                 i32,
// CHECK-NEXT:                                 ...
// CHECK-NEXT:                             ) -> LongDouble,
// CHECK-NEXT:                         >,
// CHECK-NEXT:                         *const (),
// CHECK-NEXT:                     >(sum)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]),
// CHECK-NEXT:                 2i32,
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 63]),
// CHECK-NEXT:                 third,
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"none".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_icall__rf80_f80_i32__(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<
// CHECK-NEXT:                             unsafe extern "C-unwind" fn(
// CHECK-NEXT:                                 LongDouble,
// CHECK-NEXT:                                 i32,
// CHECK-NEXT:                                 ...
// CHECK-NEXT:                             ) -> LongDouble,
// CHECK-NEXT:                         >,
// CHECK-NEXT:                         *const (),
// CHECK-NEXT:                     >(sum)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 third,
// CHECK-NEXT:                 0i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"mixed".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_icall__rf80_f80_pi8__i32_f64_f80_i32(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<
// CHECK-NEXT:                             unsafe extern "C-unwind" fn(
// CHECK-NEXT:                                 LongDouble,
// CHECK-NEXT:                                 *const i8,
// CHECK-NEXT:                                 ...
// CHECK-NEXT:                             ) -> LongDouble,
// CHECK-NEXT:                         >,
// CHECK-NEXT:                         *const (),
// CHECK-NEXT:                     >(mix)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 third,
// CHECK-NEXT:                 c"idLi".as_ptr(),
// CHECK-NEXT:                 7i32,
// CHECK-NEXT:                 0.5f64,
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:                 -2i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_icall__rv_i32__f80_f80(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, ...)>,
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                 >(print)
// CHECK-NEXT:             },
// CHECK-NEXT:             2i32,
// CHECK-NEXT:             third,
// CHECK-NEXT:             __slate_icall__rf80_f80_i32__f80(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         Option<
// CHECK-NEXT:                             unsafe extern "C-unwind" fn(
// CHECK-NEXT:                                 LongDouble,
// CHECK-NEXT:                                 i32,
// CHECK-NEXT:                                 ...
// CHECK-NEXT:                             ) -> LongDouble,
// CHECK-NEXT:                         >,
// CHECK-NEXT:                         *const (),
// CHECK-NEXT:                     >(sum)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:                 1i32,
// CHECK-NEXT:                 third,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
