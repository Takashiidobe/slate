#include <complex.h>
#include <stdio.h>

void abort(void);

static void print_lc(const char *name, long double complex z) {
  printf("%s=%Lax%Lai\n", name, creall(z), cimagl(z));
}

static long double complex mix_complex(long double complex a,
                                       long double complex b) {
  long double complex c = (a + b) / 2.0L;
  return c * 3.0L;
}

static void check_arithmetic(void) {
  long double complex a = CMPLXL(1.0L, 2.0L);
  long double complex b = CMPLXL(3.0L, -1.0L);

  print_lc("add", a + b);
  print_lc("sub", a - b);
  print_lc("mul", a * b);
  print_lc("div", a / b);
  print_lc("neg", -a);

  long double complex c  = a;
  c                     += b;
  print_lc("add_assign", c);
  c -= b;
  print_lc("sub_assign", c);
  c *= b;
  print_lc("mul_assign", c);
  c /= b;
  print_lc("div_assign", c);

  if (!(a == a))
    abort();
  if (a == b)
    abort();
  if (!(a != b))
    abort();

  print_lc("mix", mix_complex(a, b));

  __real__ a = 9.0L;
  __imag__ a = 8.0L;
  print_lc("real_imag_assign", a);
  printf("real_field=%La imag_field=%La\n", __real__ a, __imag__ a);
}

static void check_casts(void) {
  long double         ld = 5.0L;
  long double complex z  = ld;
  print_lc("real_to_complex", z);

  long double back = (long double)z;
  if (back != 5.0L)
    abort();

  long double complex nonzero_imag = CMPLXL(3.0L, 4.0L);
  long double         real_part    = (long double)nonzero_imag;
  if (real_part != 3.0L)
    abort();

  double complex zd = (double complex)nonzero_imag;
  print_lc("to_double_complex", (long double complex)zd);

  float complex zf = (float complex)nonzero_imag;
  print_lc("to_float_complex", (long double complex)zf);

  double              dd          = 6.0;
  double complex      from_double = dd;
  long double complex widened     = (long double complex)from_double;
  print_lc("from_double_complex", widened);

  int                 i32   = 7;
  long double complex fromi = i32;
  print_lc("from_int", fromi);
}

static void check_stdlib_functions(void) {
  long double complex z = CMPLXL(3.0L, 4.0L);

  printf("cabs=%La\n", cabsl(z));
  printf("carg=%La\n", cargl(z));
  print_lc("conj", conjl(z));
  print_lc("cproj", cprojl(z));
  print_lc("csqrt", csqrtl(z));
  print_lc("cexp", cexpl(CMPLXL(0.0L, 0.0L)));
  print_lc("clog", clogl(CMPLXL(1.0L, 0.0L)));
  print_lc("cpow", cpowl(CMPLXL(2.0L, 0.0L), CMPLXL(3.0L, 0.0L)));

  print_lc("csin", csinl(CMPLXL(0.0L, 0.0L)));
  print_lc("ccos", ccosl(CMPLXL(0.0L, 0.0L)));
  print_lc("ctan", ctanl(CMPLXL(0.0L, 0.0L)));
  print_lc("casin", casinl(CMPLXL(0.0L, 0.0L)));
  print_lc("cacos", cacosl(CMPLXL(1.0L, 0.0L)));
  print_lc("catan", catanl(CMPLXL(0.0L, 0.0L)));

  print_lc("csinh", csinhl(CMPLXL(0.0L, 0.0L)));
  print_lc("ccosh", ccoshl(CMPLXL(0.0L, 0.0L)));
  print_lc("ctanh", ctanhl(CMPLXL(0.0L, 0.0L)));
  print_lc("casinh", casinhl(CMPLXL(0.0L, 0.0L)));
  print_lc("cacosh", cacoshl(CMPLXL(1.0L, 0.0L)));
  print_lc("catanh", catanhl(CMPLXL(0.0L, 0.0L)));

  printf("creal=%La cimag=%La\n", creall(z), cimagl(z));
}

int main(void) {
  check_arithmetic();
  check_casts();
  check_stdlib_functions();
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct LongDouble([u8; 10]);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn add(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_add(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_sub(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_mul(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_div(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_add(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_sub(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_mul(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_div(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_neg(self)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, other: &LongDouble) -> bool {
// LOWERING-NEXT:         __slate_f80_eq(*self, *other)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-NEXT:         if __slate_f80_lt(*self, *other) {
// LOWERING-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             if __slate_f80_gt(*self, *other) {
// LOWERING-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 if __slate_f80_eq(*self, *other) {
// LOWERING-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     None
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: LongDouble,
// LOWERING-NEXT:     __slate_anon_1: LongDouble,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __divxc3(
// LOWERING-NEXT:         _0: LongDouble,
// LOWERING-NEXT:         _1: LongDouble,
// LOWERING-NEXT:         _2: LongDouble,
// LOWERING-NEXT:         _3: LongDouble,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __mulxc3(
// LOWERING-NEXT:         _0: LongDouble,
// LOWERING-NEXT:         _1: LongDouble,
// LOWERING-NEXT:         _2: LongDouble,
// LOWERING-NEXT:         _3: LongDouble,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn cabsl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn cargl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn cprojl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn csqrtl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn cexpl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn clogl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn cpowl(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         _1: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn csinl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn ccosl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn ctanl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn casinl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn cacosl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn catanl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn csinhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn ccoshl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn ctanhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn casinhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn cacoshl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn catanhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn print_lc({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut num_complex::Complex<LongDouble>) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%s=%Lax%Lai\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:         unsafe { __slate_printf__ri32_pi8_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{arg[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mix_complex(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT: ) -> {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     let mut coerce: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} = std::ptr::addr_of_mut!(coerce) as *mut {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_arithmetic() {
// LOWERING-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut a: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut b: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"add\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"sub\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval2 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval2));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"mul\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(
// LOWERING-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:         );
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval3 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval3));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"div\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_div(
// LOWERING-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval4 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval4));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"neg\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval5 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval5));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"add_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval6: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval6 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval6));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re,
// LOWERING-NEXT:         im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"sub_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval7: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval7 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval7));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(
// LOWERING-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:         );
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"mul_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval8: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval8 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval8));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_div(
// LOWERING-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"div_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval9: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval9 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval9));
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"mix\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let mut byval10: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let mut byval11: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval11 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = mix_complex(
// LOWERING-NEXT:         std::ptr::addr_of_mut!(byval10),
// LOWERING-NEXT:         std::ptr::addr_of_mut!(byval11),
// LOWERING-NEXT:     );
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval12: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval12 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval12));
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// LOWERING-NEXT:     a.re = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// LOWERING-NEXT:     a.im = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"real_imag_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let mut byval13: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval13 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval13));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"real_field=%La imag_field=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_casts() {
// LOWERING-NEXT:     let mut back: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut real_part: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"real_to_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval));
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     back = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: LongDouble = back;
// LOWERING-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     real_part = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: LongDouble = real_part;
// LOWERING-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"to_double_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval2 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval2));
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"to_float_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval3 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval3));
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 6.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"from_double_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval4 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval4));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"from_int\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval5 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval5));
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_stdlib_functions() {
// LOWERING-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce3: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce4: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce5: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce6: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce7: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce8: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce9: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce10: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce11: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce12: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce13: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce14: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce15: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce16: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce17: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// LOWERING-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cabs=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-NEXT:         __slate_cabsl__rf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"carg=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-NEXT:         __slate_cargl__rf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval2) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"conj\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval3 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval3));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cproj\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_cprojl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval4) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce17 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce17) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval5 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval5));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"csqrt\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let mut byval6: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_csqrtl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval6) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce16) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval7: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval7 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval7));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cexp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval8: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_cexpl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval8) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce15 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce15) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval9: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval9 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval9));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"clog\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval10: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_clogl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval10) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce14 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce14) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval11: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval11 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval11));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cpow\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval12: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval12 = {{_v[0-9]+}};
// LOWERING-NEXT:     let mut byval13: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval13 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_cpowl__rcf80_pcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval12) as *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval13) as *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce13 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce13) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval14: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval14 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval14));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"csin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval15: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval15 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_csinl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval15) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce12 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce12) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval16: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval16 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval16));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ccos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval17: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval17 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_ccosl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval17) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce11 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce11) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval18: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval18 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval18));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ctan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval19: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval19 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_ctanl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval19) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce10) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval20: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval20 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval20));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"casin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval21: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval21 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_casinl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval21) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce9 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce9) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval22: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval22 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval22));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cacos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval23: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval23 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_cacosl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval23) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce8) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval24: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval24 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval24));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"catan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval25: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval25 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_catanl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval25) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce7 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce7) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval26: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval26 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval26));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"csinh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval27: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval27 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_csinhl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval27) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce6) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval28: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval28 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval28));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ccosh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval29: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval29 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_ccoshl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval29) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce5) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval30: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval30 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval30));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ctanh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval31: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval31 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_ctanhl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval31) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce4) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval32: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval32 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval32));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"casinh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval33: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval33 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_casinhl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval33) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce3) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval34: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval34 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval34));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cacosh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval35: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval35 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_cacoshl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval35) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce2) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval36: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval36 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval36));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"catanh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut byval37: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval37 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe {
// LOWERING-NEXT:         __slate_catanhl__rcf80_pcf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(byval37) as *mut num_complex::Complex<LongDouble>
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut byval38: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: LongDouble([0; 10]),
// LOWERING-NEXT:         im: LongDouble([0; 10]),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     byval38 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval38));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"creal=%La cimag=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     check_arithmetic();
// LOWERING-NEXT:     check_casts();
// LOWERING-NEXT:     check_stdlib_functions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __slate_cabsl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn __slate_cacoshl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cacosl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cargl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn __slate_casinhl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_casinl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_catanhl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_catanl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ccoshl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ccosl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// LOWERING-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cexpl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// LOWERING-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     safe fn __slate_cf80_div(
// LOWERING-NEXT:         a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-NEXT:         a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __slate_clogl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// LOWERING-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cpowl__rcf80_pcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         _1: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cprojl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_csinhl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_csinl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// LOWERING-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_csqrtl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ctanhl__rcf80_pcf80(
// LOWERING-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ctanl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// LOWERING-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// LOWERING-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// LOWERING-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// LOWERING-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// LOWERING-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// LOWERING-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// LOWERING-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// LOWERING-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// LOWERING-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// LOWERING-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// LOWERING-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *mut i8, _1: LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(_0: *mut i8, _1: LongDouble, _2: LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80_f80(
// LOWERING-NEXT:         _0: *mut i8,
// LOWERING-NEXT:         _1: *mut i8,
// LOWERING-NEXT:         _2: LongDouble,
// LOWERING-NEXT:         _3: LongDouble,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct LongDouble([u8; 10]);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn add(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_add(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_sub(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_mul(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_div(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_add(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_sub(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_mul(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_div(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_neg(self)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, other: &LongDouble) -> bool {
// REWRITES-NEXT:         __slate_f80_eq(*self, *other)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-NEXT:         if __slate_f80_lt(*self, *other) {
// REWRITES-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             if __slate_f80_gt(*self, *other) {
// REWRITES-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-NEXT:             } else {
// REWRITES-NEXT:                 if __slate_f80_eq(*self, *other) {
// REWRITES-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     None
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:     __slate_anon_0: LongDouble,
// REWRITES-NEXT:     __slate_anon_1: LongDouble,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __divxc3(
// REWRITES-NEXT:         _0: LongDouble,
// REWRITES-NEXT:         _1: LongDouble,
// REWRITES-NEXT:         _2: LongDouble,
// REWRITES-NEXT:         _3: LongDouble,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __mulxc3(
// REWRITES-NEXT:         _0: LongDouble,
// REWRITES-NEXT:         _1: LongDouble,
// REWRITES-NEXT:         _2: LongDouble,
// REWRITES-NEXT:         _3: LongDouble,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn cabsl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn cargl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn cprojl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn csqrtl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn cexpl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn clogl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn cpowl(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         _1: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn csinl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn ccosl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn ctanl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn casinl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn cacosl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn catanl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn csinhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn ccoshl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn ctanhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn casinhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn cacoshl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn catanhl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn print_lc({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: &num_complex::Complex<LongDouble>) {
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         unsafe { *({{arg[0-9]+}} as *const num_complex::Complex<LongDouble>) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_printf__ri32_pi8_pi8_f80_f80(
// REWRITES-NEXT:             c"%s=%Lax%Lai\n".as_ptr() as *mut i8,
// REWRITES-NEXT:             {{arg[0-9]+}} as *mut i8,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mix_complex(
// REWRITES-NEXT:     {{arg[0-9]+}}: &num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     {{arg[0-9]+}}: &num_complex::Complex<LongDouble>,
// REWRITES-NEXT: ) -> {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:     let mut coerce: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0; 10]),
// REWRITES-NEXT:         im: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         unsafe { *({{arg[0-9]+}} as *const num_complex::Complex<LongDouble>) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         unsafe { *({{arg[0-9]+}} as *const num_complex::Complex<LongDouble>) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re / {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im / {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     coerce = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re * {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im * {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} = std::ptr::addr_of_mut!(coerce) as *mut {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     return unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_arithmetic() {
// REWRITES-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut a: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut b: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"add".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"sub".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval2)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"mul".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} - {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} != {{_v[0-9]+}} { {{_v[0-9]+}} != {{_v[0-9]+}} } else { false };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(
// REWRITES-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:         );
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval3)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"div".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_div(
// REWRITES-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval4)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"neg".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: -{{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: -{{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval5)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"add_assign".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval6: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval6)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"sub_assign".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval7: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval7)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} - {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} != {{_v[0-9]+}} { {{_v[0-9]+}} != {{_v[0-9]+}} } else { false };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(
// REWRITES-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:             num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:         );
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"mul_assign".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval8: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval8)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_div(
// REWRITES-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:         num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} },
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"div_assign".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval9: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval9)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = !(a == a);
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = a == b;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = !(a != b);
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"mix".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT:     let mut byval10: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     let mut byval11: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce = mix_complex(unsafe { &(*std::ptr::addr_of_mut!(byval10)) }, unsafe {
// REWRITES-NEXT:         &(*std::ptr::addr_of_mut!(byval11))
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval12: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval12)) });
// REWRITES-NEXT:     a.re = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// REWRITES-NEXT:     a.im = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"real_imag_assign".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let mut byval13: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval13)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_printf__ri32_pi8_f80_f80(
// REWRITES-NEXT:             c"real_field=%La imag_field=%La\n".as_ptr() as *mut i8,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_casts() {
// REWRITES-NEXT:     let mut back: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut real_part: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]),
// REWRITES-NEXT:         im: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"real_to_complex".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval)) });
// REWRITES-NEXT:     back = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = back != LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     real_part = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = real_part != LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"to_double_complex".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval2)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"to_float_complex".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval3)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = 6.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"from_double_complex".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval4)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32(7 as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}},
// REWRITES-NEXT:         im: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"from_int".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval5)) });
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_stdlib_functions() {
// REWRITES-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce3: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce4: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce5: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce6: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce7: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce8: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce9: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce10: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce11: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce12: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce13: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce14: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce15: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce16: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut coerce17: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:         __slate_anon_0: LongDouble([0; 10]),
// REWRITES-NEXT:         __slate_anon_1: LongDouble([0; 10]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"cabs=%La\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, unsafe {
// REWRITES-NEXT:             __slate_cabsl__rf80_pcf80(
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(byval) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:             )
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"carg=%La\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, unsafe {
// REWRITES-NEXT:             __slate_cargl__rf80_pcf80(
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(byval2) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:             )
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"conj".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: -{{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval3)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"cproj".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce17 = unsafe {
// REWRITES-NEXT:         __slate_cprojl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval4) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce17) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval5)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"csqrt".as_ptr() as *mut i8;
// REWRITES-NEXT:     let mut byval6: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce16 = unsafe {
// REWRITES-NEXT:         __slate_csqrtl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval6) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce16) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval7: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval7)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"cexp".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval8: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce15 = unsafe {
// REWRITES-NEXT:         __slate_cexpl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval8) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce15) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval9: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval9)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"clog".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval10: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce14 = unsafe {
// REWRITES-NEXT:         __slate_clogl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval10) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce14) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval11: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval11)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"cpow".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval12: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     let mut byval13: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce13 = unsafe {
// REWRITES-NEXT:         __slate_cpowl__rcf80_pcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval12) as *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval13) as *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce13) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval14: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval14)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"csin".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval15: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce12 = unsafe {
// REWRITES-NEXT:         __slate_csinl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval15) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce12) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval16: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval16)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"ccos".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval17: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce11 = unsafe {
// REWRITES-NEXT:         __slate_ccosl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval17) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce11) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval18: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval18)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"ctan".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval19: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce10 = unsafe {
// REWRITES-NEXT:         __slate_ctanl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval19) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce10) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval20: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval20)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"casin".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval21: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce9 = unsafe {
// REWRITES-NEXT:         __slate_casinl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval21) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce9) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval22: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval22)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"cacos".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval23: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce8 = unsafe {
// REWRITES-NEXT:         __slate_cacosl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval23) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce8) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval24: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval24)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"catan".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval25: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce7 = unsafe {
// REWRITES-NEXT:         __slate_catanl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval25) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce7) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval26: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval26)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"csinh".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval27: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce6 = unsafe {
// REWRITES-NEXT:         __slate_csinhl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval27) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce6) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval28: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval28)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"ccosh".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval29: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce5 = unsafe {
// REWRITES-NEXT:         __slate_ccoshl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval29) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce5) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval30: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval30)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"ctanh".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval31: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce4 = unsafe {
// REWRITES-NEXT:         __slate_ctanhl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval31) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce4) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval32: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval32)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"casinh".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval33: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce3 = unsafe {
// REWRITES-NEXT:         __slate_casinhl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval33) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce3) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval34: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval34)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"cacosh".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval35: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce2 = unsafe {
// REWRITES-NEXT:         __slate_cacoshl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval35) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce2) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval36: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval36)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"catanh".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut byval37: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     coerce = unsafe {
// REWRITES-NEXT:         __slate_catanhl__rcf80_pcf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(byval37) as *mut num_complex::Complex<LongDouble>
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let mut byval38: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT:     print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval38)) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_printf__ri32_pi8_f80_f80(c"creal=%La cimag=%La\n".as_ptr() as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}})
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     check_arithmetic();
// REWRITES-NEXT:     check_casts();
// REWRITES-NEXT:     check_stdlib_functions();
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __slate_cabsl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn __slate_cacoshl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cacosl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cargl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn __slate_casinhl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_casinl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_catanhl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_catanl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ccoshl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ccosl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// REWRITES-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cexpl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// REWRITES-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     safe fn __slate_cf80_div(
// REWRITES-NEXT:         a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-NEXT:         a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __slate_clogl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// REWRITES-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cpowl__rcf80_pcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         _1: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cprojl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_csinhl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_csinl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// REWRITES-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_csqrtl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ctanhl__rcf80_pcf80(
// REWRITES-NEXT:         _0: *mut num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ctanl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>)
// REWRITES-NEXT:     -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// REWRITES-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// REWRITES-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// REWRITES-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// REWRITES-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// REWRITES-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// REWRITES-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// REWRITES-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// REWRITES-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// REWRITES-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// REWRITES-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *mut i8, _1: LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(_0: *mut i8, _1: LongDouble, _2: LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80_f80(
// REWRITES-NEXT:         _0: *mut i8,
// REWRITES-NEXT:         _1: *mut i8,
// REWRITES-NEXT:         _2: LongDouble,
// REWRITES-NEXT:         _3: LongDouble,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
