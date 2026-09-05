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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f128)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-LOWERING-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-LOWERING-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-LOWERING-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-LOWERING-NEXT:     fn __multc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-LOWERING-NEXT:     fn __divtc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn abort() -> !;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn cpowl(
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     check_arithmetic();
// COMMON-LOWERING-NEXT:     check_casts();
// COMMON-LOWERING-NEXT:     check_stdlib_functions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: fn check_arithmetic() {
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}}.re - {{__v[0-9]+}}.re,
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}}.im - {{__v[0-9]+}}.im,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}}.re - {{__v[0-9]+}}.re,
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}}.im - {{__v[0-9]+}}.im,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             unsafe { abort() };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             unsafe { abort() };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             unsafe { abort() };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     a.re = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     a.im = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: fn check_casts() {
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     back = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             unsafe { abort() };
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     real_part = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             unsafe { abort() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 6.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: fn check_stdlib_functions() {
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}}.re,
// COMMON-LOWERING-NEXT:         im: -{{__v[0-9]+}}.im,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: fn mix_complex(
// COMMON-LOWERING-NEXT:         re: {{arg[0-9]+}}.re + {{arg[0-9]+}}.re,
// COMMON-LOWERING-NEXT:         im: {{arg[0-9]+}}.im + {{arg[0-9]+}}.im,
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn cabsl(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn cargl(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn cprojl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn csqrtl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn cexpl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn clogl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         _1: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn csinl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn ccosl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn ctanl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn casinl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn cacosl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn catanl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn csinhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn ccoshl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn ctanhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn casinhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn cacoshl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn catanhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut a: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     let mut b: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"add\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sub\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"mul\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"div\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"neg\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = -{{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = -{{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"add_assign\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sub_assign\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"mul_assign\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:             num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-NEXT:             },
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"div_assign\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"mix\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = mix_complex({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"real_imag_assign\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"real_field=%La imag_field=%La\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{__v[0-9]+}} as *mut i8, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut back: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut real_part: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"real_to_complex\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: LongDouble = back;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: LongDouble = real_part;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"to_double_complex\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = __slate_f80_to_f32({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = __slate_f80_to_f32({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"to_float_complex\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"from_double_complex\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"from_int\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cabs=%La\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_cabsl__rf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{__v[0-9]+}} as *mut i8, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"carg=%La\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_cargl__rf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{__v[0-9]+}} as *mut i8, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"conj\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cproj\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_cprojl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"csqrt\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_csqrtl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cexp\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_cexpl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"clog\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_clogl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cpow\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_cpowl__rcf80_cf80_cf80(
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"csin\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_csinl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ccos\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_ccosl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ctan\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_ctanl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"casin\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_casinl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cacos\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_cacosl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"catan\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_catanl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"csinh\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_csinhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ccosh\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_ccoshl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ctanh\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_ctanhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"casinh\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_casinhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cacosh\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_cacoshl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"catanh\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_catanhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"creal=%La cimag=%La\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{__v[0-9]+}} as *mut i8, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn print_lc({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: num_complex::Complex<LongDouble>) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s=%Lax%Lai\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_printf__ri32_pi8_pi8_f80_f80({{__v[0-9]+}} as *mut i8, {{arg[0-9]+}} as *mut i8, {{__v[0-9]+}}, {{__v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT: ) -> num_complex::Complex<LongDouble> {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cabsl__rf80_cf80(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cacoshl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cacosl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cargl__rf80_cf80(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_casinhl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_casinl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_catanhl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_catanl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ccoshl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ccosl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cexpl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_clogl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cpowl__rcf80_cf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         _1: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_cprojl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_csinhl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_csinl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_csqrtl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ctanhl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ctanl__rcf80_cf80(
// LOWERING-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *mut i8, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(_0: *mut i8, _1: LongDouble, _2: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _1: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     fn cabsl(_0: num_complex::Complex<f128>) -> f128;
// LOWERING-AARCH64-GNU-NEXT:     fn cargl(_0: num_complex::Complex<f128>) -> f128;
// LOWERING-AARCH64-GNU-NEXT:     fn cprojl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn csqrtl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn cexpl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn clogl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:         _0: num_complex::Complex<f128>,
// LOWERING-AARCH64-GNU-NEXT:         _1: num_complex::Complex<f128>,
// LOWERING-AARCH64-GNU-NEXT:     ) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn csinl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn ccosl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn ctanl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn casinl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn cacosl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn catanl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn csinhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn ccoshl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn ctanhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn casinhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn cacoshl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     fn catanhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// LOWERING-AARCH64-GNU-NEXT:     let mut a: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:     let mut b: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.0f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 2.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: -1.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"add\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"sub\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"mul\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-NEXT:         unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"div\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-NEXT:         unsafe { __divtc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"neg\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = -{{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = -{{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"add_assign\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"sub_assign\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-NEXT:         unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"mul_assign\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-NEXT:         unsafe { __divtc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"div_assign\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"mix\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = mix_complex({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 9.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 8.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"real_imag_assign\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"real_field=%La imag_field=%La\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT:     let mut back: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let mut real_part: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"real_to_complex\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f128 = back;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 4.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f128 = real_part;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f128 = 3.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re as f64;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im as f64;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"to_double_complex\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re as f32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im as f32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"to_float_complex\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"from_double_complex\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.0f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"from_int\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 4.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cabs=%La\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { cabsl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"carg=%La\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { cargl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"conj\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cproj\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cprojl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"csqrt\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { csqrtl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cexp\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cexpl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"clog\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { clogl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cpow\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 2.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe {
// LOWERING-AARCH64-GNU-NEXT:         cpowl(
// LOWERING-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<f128>,
// LOWERING-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<f128>,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"csin\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { csinl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ccos\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { ccosl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ctan\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { ctanl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"casin\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { casinl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cacos\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cacosl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"catan\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { catanl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"csinh\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { csinhl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ccosh\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { ccoshl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ctanh\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { ctanhl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"casinh\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { casinhl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cacosh\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { cacoshl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"catanh\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { catanhl({{__v[0-9]+}} as num_complex::Complex<f128>) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"creal=%La cimag=%La\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT: fn print_lc({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: num_complex::Complex<f128>) {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s=%Lax%Lai\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<f128>,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<f128>,
// LOWERING-AARCH64-GNU-NEXT: ) -> num_complex::Complex<f128> {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(f128)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-REWRITES-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-REWRITES-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-REWRITES-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-REWRITES-NEXT:     fn __multc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-REWRITES-NEXT:     fn __divtc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn abort() -> !;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn cpowl(
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     check_arithmetic();
// COMMON-REWRITES-NEXT:     check_casts();
// COMMON-REWRITES-NEXT:     check_stdlib_functions();
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: fn check_arithmetic() {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re - {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im - {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:         re: -{{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: -{{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re + {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im + {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re - {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im - {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !(a == a);
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { abort() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = a == b;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { abort() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !(a != b);
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { abort() };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: fn check_casts() {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     back = {{__v[0-9]+}}.re;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { abort() };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     real_part = {{__v[0-9]+}}.re;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         unsafe { abort() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 6.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: fn check_stdlib_functions() {
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: -{{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     });
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: fn mix_complex(
// COMMON-REWRITES-NEXT:         re: {{arg[0-9]+}}.re + {{arg[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{arg[0-9]+}}.im + {{arg[0-9]+}}.im,
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re / {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im / {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re * {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im * {{__v[0-9]+}},
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn cabsl(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn cargl(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn cprojl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn csqrtl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn cexpl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn clogl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         _1: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn csinl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn ccosl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn ctanl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn casinl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn cacosl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn catanl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn csinhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn ccoshl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn ctanhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn casinhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn cacoshl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn catanhl(_0: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut a: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// REWRITES-X86_64-GNU-NEXT:     let mut b: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"add".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"sub".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"mul".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"div".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"neg".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"add_assign".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"sub_assign".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"mul_assign".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"div_assign".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:             num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-NEXT:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-NEXT:             },
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"mix".as_ptr() as *mut i8, mix_complex(a, b));
// REWRITES-X86_64-GNU-NEXT:     a.re = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:     a.im = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"real_imag_assign".as_ptr() as *mut i8, a);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-NEXT:         __slate_printf__ri32_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:             c"real_field=%La imag_field=%La\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut back: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut real_part: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"real_to_complex".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = back != LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = real_part != LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f64 = __slate_f80_to_f64({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"to_double_complex".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = __slate_f80_to_f32({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = __slate_f80_to_f32({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         im: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"to_float_complex".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         im: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     print_lc({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         im: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f64({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         im: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"from_double_complex".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(7 as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"from_int".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// REWRITES-X86_64-GNU-NEXT:         __slate_printf__ri32_pi8_f80(c"cabs=%La\n".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_cabsl__rf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_printf__ri32_pi8_f80(c"carg=%La\n".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_cargl__rf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"conj".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"cproj".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cprojl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"csqrt".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_csqrtl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"cexp".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cexpl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"clog".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_clogl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"cpow".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cpowl__rcf80_cf80_cf80(
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"csin".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_csinl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"ccos".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_ccosl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"ctan".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_ctanl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"casin".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_casinl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"cacos".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cacosl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"catan".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_catanl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"csinh".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_csinhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"ccosh".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_ccoshl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"ctanh".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_ctanhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"casinh".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_casinhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"cacosh".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_cacoshl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:         im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-NEXT:     print_lc(c"catanh".as_ptr() as *mut i8, unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_catanhl__rcf80_cf80({{__v[0-9]+}} as num_complex::Complex<LongDouble>)
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_printf__ri32_pi8_f80_f80(c"creal=%La cimag=%La\n".as_ptr() as *mut i8, {{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn print_lc({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: num_complex::Complex<LongDouble>) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}}.re;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}}.im;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_printf__ri32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:             c"%s=%Lax%Lai\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             {{arg[0-9]+}} as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT: ) -> num_complex::Complex<LongDouble> {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cabsl__rf80_cf80(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cacoshl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cacosl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cargl__rf80_cf80(_0: num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_casinhl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_casinl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_catanhl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_catanl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ccoshl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ccosl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cexpl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_clogl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cpowl__rcf80_cf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         _1: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_cprojl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_csinhl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_csinl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_csqrtl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ctanhl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ctanl__rcf80_cf80(
// REWRITES-X86_64-GNU-NEXT:         _0: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *mut i8, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(_0: *mut i8, _1: LongDouble, _2: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _1: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     fn cabsl(_0: num_complex::Complex<f128>) -> f128;
// REWRITES-AARCH64-GNU-NEXT:     fn cargl(_0: num_complex::Complex<f128>) -> f128;
// REWRITES-AARCH64-GNU-NEXT:     fn cprojl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn csqrtl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn cexpl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn clogl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:         _0: num_complex::Complex<f128>,
// REWRITES-AARCH64-GNU-NEXT:         _1: num_complex::Complex<f128>,
// REWRITES-AARCH64-GNU-NEXT:     ) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn csinl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn ccosl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn ctanl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn casinl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn cacosl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn catanl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn csinhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn ccoshl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn ctanhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn casinhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn cacoshl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     fn catanhl(_0: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// REWRITES-AARCH64-GNU-NEXT:     let mut a: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:     let mut b: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.0f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 2.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     a = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: -1.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     b = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"add".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"sub".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"mul".as_ptr() as *mut u8, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"div".as_ptr() as *mut u8, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         __divtc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"neg".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"add_assign".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"sub_assign".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> =
// REWRITES-AARCH64-GNU-NEXT:         unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"mul_assign".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = b;
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"div_assign".as_ptr() as *mut u8, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         __divtc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im)
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"mix".as_ptr() as *mut u8, mix_complex(a, b));
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 9.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     a.re = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 8.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     a.im = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"real_imag_assign".as_ptr() as *mut u8, a);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = a;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-NEXT:     unsafe { printf(c"real_field=%La imag_field=%La\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-AARCH64-GNU-NEXT:     let mut back: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let mut real_part: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"real_to_complex".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = back;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 4.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = real_part;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f64,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f64,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f128,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"to_double_complex".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f32,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f32,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f128,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"to_float_complex".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f128,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"from_double_complex".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = (7 as i32) as f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.0f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"from_int".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 4.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         printf(c"cabs=%La\n".as_ptr(), unsafe {
// REWRITES-AARCH64-GNU-NEXT:             cabsl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:         printf(c"carg=%La\n".as_ptr(), unsafe {
// REWRITES-AARCH64-GNU-NEXT:             cargl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"conj".as_ptr() as *mut u8, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"cproj".as_ptr() as *mut u8, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         cprojl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     print_lc(c"csqrt".as_ptr() as *mut u8, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         csqrtl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"cexp".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe { cexpl({{__v[0-9]+}} as num_complex::Complex<f128>) });
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"clog".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe { clogl({{__v[0-9]+}} as num_complex::Complex<f128>) });
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"cpow".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 2.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 3.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         cpowl(
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<f128>,
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as num_complex::Complex<f128>,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"csin".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe { csinl({{__v[0-9]+}} as num_complex::Complex<f128>) });
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"ccos".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe { ccosl({{__v[0-9]+}} as num_complex::Complex<f128>) });
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"ctan".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe { ctanl({{__v[0-9]+}} as num_complex::Complex<f128>) });
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"casin".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         casinl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"cacos".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         cacosl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"catan".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         catanl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"csinh".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         csinhl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"ccosh".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         ccoshl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"ctanh".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         ctanhl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"casinh".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         casinhl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"cacosh".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 1.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         cacoshl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"catanh".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:         im: 0.000000e+00f128,
// REWRITES-AARCH64-GNU-NEXT:     print_lc({{__v[0-9]+}}, unsafe {
// REWRITES-AARCH64-GNU-NEXT:         catanhl({{__v[0-9]+}} as num_complex::Complex<f128>)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-NEXT:     unsafe { printf(c"creal=%La cimag=%La\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-AARCH64-GNU-NEXT: fn print_lc({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: num_complex::Complex<f128>) {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}}.re;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{arg[0-9]+}}.im;
// REWRITES-AARCH64-GNU-NEXT:     unsafe { printf(c"%s=%Lax%Lai\n".as_ptr(), {{arg[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<f128>,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: num_complex::Complex<f128>,
// REWRITES-AARCH64-GNU-NEXT: ) -> num_complex::Complex<f128> {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 2.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.000000e+00f128;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
