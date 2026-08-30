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

  long double complex c = a;
  c += b;
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
  long double ld       = 5.0L;
  long double complex z = ld;
  print_lc("real_to_complex", z);

  long double back = (long double)z;
  if (back != 5.0L)
    abort();

  long double complex nonzero_imag = CMPLXL(3.0L, 4.0L);
  long double real_part            = (long double)nonzero_imag;
  if (real_part != 3.0L)
    abort();

  double complex zd = (double complex)nonzero_imag;
  print_lc("to_double_complex", (long double complex)zd);

  float complex zf = (float complex)nonzero_imag;
  print_lc("to_float_complex", (long double complex)zf);

  double dd                       = 6.0;
  double complex from_double      = dd;
  long double complex widened     = (long double complex)from_double;
  print_lc("from_double_complex", widened);

  int i32                    = 7;
  long double complex fromi  = i32;
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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT:     fn add(self, o: LongDouble) -> LongDouble { __slate_f80_add(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, o: LongDouble) -> LongDouble { __slate_f80_sub(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, o: LongDouble) -> LongDouble { __slate_f80_mul(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, o: LongDouble) -> LongDouble { __slate_f80_div(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_add(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_sub(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_mul(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_div(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble { __slate_f80_neg(self) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, other: &LongDouble) -> bool { __slate_f80_eq(*self, *other) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> { if __slate_f80_lt(*self, *other) { Some(std::cmp::Ordering::Less) } else { if __slate_f80_gt(*self, *other) { Some(std::cmp::Ordering::Greater) } else { if __slate_f80_eq(*self, *other) { Some(std::cmp::Ordering::Equal) } else { None } } } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: LongDouble,
// LOWERING-NEXT:     __slate_anon_1: LongDouble,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __divxc3(_0: LongDouble, _1: LongDouble, _2: LongDouble, _3: LongDouble) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __mulxc3(_0: LongDouble, _1: LongDouble, _2: LongDouble, _3: LongDouble) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn cabsl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn cargl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn cprojl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn csqrtl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn cexpl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn clogl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn cpowl(_0: *mut num_complex::Complex<LongDouble>, _1: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
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
// LOWERING-NEXT:     let mut name: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut z: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     name = {{arg[0-9]+}};
// LOWERING-NEXT:     z = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%s=%Lax%Lai\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = name;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mix_complex({{arg[0-9]+}}: *mut num_complex::Complex<LongDouble>, {{arg[0-9]+}}: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     let mut coerce: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let mut a: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut b: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut __retval: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut c: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __retval;
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} = std::ptr::addr_of_mut!(coerce) as *mut {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_arithmetic() {
// LOWERING-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut a: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut b: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce6: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut c: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce7: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce8: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce9: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce10: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce11: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce12: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce13: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce14: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]) };
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]) };
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"add\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// LOWERING-NEXT:     coerce2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce2;
// LOWERING-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"sub\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im };
// LOWERING-NEXT:     coerce3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce3;
// LOWERING-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
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
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce4;
// LOWERING-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval3 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval3));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"div\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_div(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// LOWERING-NEXT:     coerce5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce5;
// LOWERING-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval4 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval4));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"neg\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     coerce6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce6;
// LOWERING-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval5 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval5));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"add_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     coerce7 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce7;
// LOWERING-NEXT:     let mut byval6: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval6 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval6));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im };
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"sub_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     coerce8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce8;
// LOWERING-NEXT:     let mut byval7: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval7 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval7));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
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
// LOWERING-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"mul_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     coerce9 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce9;
// LOWERING-NEXT:     let mut byval8: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval8 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval8));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_div(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"div_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// LOWERING-NEXT:     coerce10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce10;
// LOWERING-NEXT:     let mut byval9: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
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
// LOWERING-NEXT:     coerce11 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce11;
// LOWERING-NEXT:     coerce12 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce12;
// LOWERING-NEXT:     let mut byval10: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let mut byval11: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval11 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = mix_complex(std::ptr::addr_of_mut!(byval10), std::ptr::addr_of_mut!(byval11));
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce13 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce13;
// LOWERING-NEXT:     let mut byval12: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval12 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval12));
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// LOWERING-NEXT:     a.re = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// LOWERING-NEXT:     a.im = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"real_imag_assign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// LOWERING-NEXT:     coerce14 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce14;
// LOWERING-NEXT:     let mut byval13: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
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
// LOWERING-NEXT:     let mut ld: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut z: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut back: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut nonzero_imag: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut real_part: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut zd: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut coerce2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut zf: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut coerce3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut dd: f64 = 0.0;
// LOWERING-NEXT:     let mut from_double: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut widened: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut i32: i32 = 0;
// LOWERING-NEXT:     let mut fromi: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     ld = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = ld;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     z = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"real_to_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce;
// LOWERING-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]) };
// LOWERING-NEXT:     nonzero_imag = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = nonzero_imag;
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = nonzero_imag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     zd = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"to_double_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = zd;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     coerce2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce2;
// LOWERING-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval2 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval2));
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = nonzero_imag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     zf = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"to_float_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = zf;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     coerce3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce3;
// LOWERING-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval3 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval3));
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 6.0;
// LOWERING-NEXT:     dd = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = dd;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     from_double = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = from_double;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     widened = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"from_double_complex\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = widened;
// LOWERING-NEXT:     coerce4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce4;
// LOWERING-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval4 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval4));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     i32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     fromi = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"from_int\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = fromi;
// LOWERING-NEXT:     coerce5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce5;
// LOWERING-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval5 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval5));
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_stdlib_functions() {
// LOWERING-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce3: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce4: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce5: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce6: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce7: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce8: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce9: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce10: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce11: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce12: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce13: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce14: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce15: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce16: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce17: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut z: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce18: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce19: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce20: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce21: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce22: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce23: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce24: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce25: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce26: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce27: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce28: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce29: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce30: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce31: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce32: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce33: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce34: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce35: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce36: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce37: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce38: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce39: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce40: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce41: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce42: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce43: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce44: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce45: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce46: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce47: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce48: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce49: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce50: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce51: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce52: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce53: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce54: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut coerce55: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]) };
// LOWERING-NEXT:     z = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cabs=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     coerce18 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce18;
// LOWERING-NEXT:     let mut byval: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_cabsl__rf80_pcf80(std::ptr::addr_of_mut!(byval) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"carg=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     coerce19 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce19;
// LOWERING-NEXT:     let mut byval2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_cargl__rf80_pcf80(std::ptr::addr_of_mut!(byval2) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"conj\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     coerce20 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce20;
// LOWERING-NEXT:     let mut byval3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval3 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval3));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cproj\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     coerce21 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce21;
// LOWERING-NEXT:     let mut byval4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_cprojl__rcf80_pcf80(std::ptr::addr_of_mut!(byval4) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce17 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce17) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce22 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce22;
// LOWERING-NEXT:     let mut byval5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval5 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval5));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"csqrt\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     coerce23 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce23;
// LOWERING-NEXT:     let mut byval6: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_csqrtl__rcf80_pcf80(std::ptr::addr_of_mut!(byval6) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce16) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce24 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce24;
// LOWERING-NEXT:     let mut byval7: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval7 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval7));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cexp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce25 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce25;
// LOWERING-NEXT:     let mut byval8: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_cexpl__rcf80_pcf80(std::ptr::addr_of_mut!(byval8) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce15 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce15) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce26 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce26;
// LOWERING-NEXT:     let mut byval9: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval9 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval9));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"clog\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce27 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce27;
// LOWERING-NEXT:     let mut byval10: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_clogl__rcf80_pcf80(std::ptr::addr_of_mut!(byval10) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce14 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce14) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce28 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce28;
// LOWERING-NEXT:     let mut byval11: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval11 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval11));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cpow\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce29 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce29;
// LOWERING-NEXT:     coerce30 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce30;
// LOWERING-NEXT:     let mut byval12: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval12 = {{_v[0-9]+}};
// LOWERING-NEXT:     let mut byval13: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval13 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_cpowl__rcf80_pcf80_pcf80(std::ptr::addr_of_mut!(byval12) as *mut num_complex::Complex<LongDouble>, std::ptr::addr_of_mut!(byval13) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce13 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce13) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce31 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce31;
// LOWERING-NEXT:     let mut byval14: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval14 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval14));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"csin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce32;
// LOWERING-NEXT:     let mut byval15: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval15 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_csinl__rcf80_pcf80(std::ptr::addr_of_mut!(byval15) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce12 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce12) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce33 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce33;
// LOWERING-NEXT:     let mut byval16: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval16 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval16));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ccos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce34 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce34;
// LOWERING-NEXT:     let mut byval17: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval17 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_ccosl__rcf80_pcf80(std::ptr::addr_of_mut!(byval17) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce11 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce11) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce35 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce35;
// LOWERING-NEXT:     let mut byval18: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval18 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval18));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ctan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce36 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce36;
// LOWERING-NEXT:     let mut byval19: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval19 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_ctanl__rcf80_pcf80(std::ptr::addr_of_mut!(byval19) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce10 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce10) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce37 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce37;
// LOWERING-NEXT:     let mut byval20: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval20 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval20));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"casin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce38 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce38;
// LOWERING-NEXT:     let mut byval21: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval21 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_casinl__rcf80_pcf80(std::ptr::addr_of_mut!(byval21) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce9 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce9) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce39 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce39;
// LOWERING-NEXT:     let mut byval22: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval22 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval22));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cacos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce40 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce40;
// LOWERING-NEXT:     let mut byval23: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval23 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_cacosl__rcf80_pcf80(std::ptr::addr_of_mut!(byval23) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce8) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce41 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce41;
// LOWERING-NEXT:     let mut byval24: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval24 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval24));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"catan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce42 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce42;
// LOWERING-NEXT:     let mut byval25: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval25 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_catanl__rcf80_pcf80(std::ptr::addr_of_mut!(byval25) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce7 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce7) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce43 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce43;
// LOWERING-NEXT:     let mut byval26: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval26 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval26));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"csinh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce44 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce44;
// LOWERING-NEXT:     let mut byval27: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval27 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_csinhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval27) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce6) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce45 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce45;
// LOWERING-NEXT:     let mut byval28: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval28 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval28));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ccosh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce46 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce46;
// LOWERING-NEXT:     let mut byval29: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval29 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_ccoshl__rcf80_pcf80(std::ptr::addr_of_mut!(byval29) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce5) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce47 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce47;
// LOWERING-NEXT:     let mut byval30: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval30 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval30));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ctanh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce48 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce48;
// LOWERING-NEXT:     let mut byval31: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval31 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_ctanhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval31) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce4) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce49 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce49;
// LOWERING-NEXT:     let mut byval32: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval32 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval32));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"casinh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce50 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce50;
// LOWERING-NEXT:     let mut byval33: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval33 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_casinhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval33) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce3) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce51 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce51;
// LOWERING-NEXT:     let mut byval34: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval34 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval34));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"cacosh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce52 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce52;
// LOWERING-NEXT:     let mut byval35: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval35 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_cacoshl__rcf80_pcf80(std::ptr::addr_of_mut!(byval35) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce2) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce53 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce53;
// LOWERING-NEXT:     let mut byval36: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval36 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval36));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"catanh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// LOWERING-NEXT:     coerce54 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce54;
// LOWERING-NEXT:     let mut byval37: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval37 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = unsafe { __slate_catanhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval37) as *mut num_complex::Complex<LongDouble>) };
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     coerce55 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce55;
// LOWERING-NEXT:     let mut byval38: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     byval38 = {{_v[0-9]+}};
// LOWERING-NEXT:     print_lc({{_v[0-9]+}}, std::ptr::addr_of_mut!(byval38));
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"creal=%La cimag=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     check_arithmetic();
// LOWERING-NEXT:     check_casts();
// LOWERING-NEXT:     check_stdlib_functions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __slate_cabsl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn __slate_cacoshl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cacosl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cargl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// LOWERING-NEXT:     fn __slate_casinhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_casinl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_catanhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_catanl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ccoshl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ccosl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cexpl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     safe fn __slate_cf80_div(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __slate_clogl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cpowl__rcf80_pcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>, _1: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_cprojl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_csinhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_csinl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_csqrtl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ctanhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     fn __slate_ctanl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
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
// LOWERING-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80_f80(_0: *mut i8, _1: *mut i8, _2: LongDouble, _3: LongDouble) -> i32;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-NEXT:     fn add(self, o: LongDouble) -> LongDouble { __slate_f80_add(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, o: LongDouble) -> LongDouble { __slate_f80_sub(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, o: LongDouble) -> LongDouble { __slate_f80_mul(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, o: LongDouble) -> LongDouble { __slate_f80_div(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_add(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_sub(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_mul(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_div(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble { __slate_f80_neg(self) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, other: &LongDouble) -> bool { __slate_f80_eq(*self, *other) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> { if __slate_f80_lt(*self, *other) { Some(std::cmp::Ordering::Less) } else { if __slate_f80_gt(*self, *other) { Some(std::cmp::Ordering::Greater) } else { if __slate_f80_eq(*self, *other) { Some(std::cmp::Ordering::Equal) } else { None } } } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:     __slate_anon_0: LongDouble,
// REWRITES-NEXT:     __slate_anon_1: LongDouble,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __divxc3(_0: LongDouble, _1: LongDouble, _2: LongDouble, _3: LongDouble) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __mulxc3(_0: LongDouble, _1: LongDouble, _2: LongDouble, _3: LongDouble) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn cabsl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn cargl(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn cprojl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn csqrtl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn cexpl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn clogl(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn cpowl(_0: *mut num_complex::Complex<LongDouble>, _1: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
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
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *({{arg[0-9]+}} as *const num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let mut name: *mut i8 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut z: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%s=%Lax%Lai\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_pi8_f80_f80({{_v[0-9]+}} as *mut i8, name as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mix_complex({{arg[0-9]+}}: &num_complex::Complex<LongDouble>, {{arg[0-9]+}}: &num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT: let mut coerce: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *({{arg[0-9]+}} as *const num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe { *({{arg[0-9]+}} as *const num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let mut a: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let mut b: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let mut __retval: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut c: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: c = num_complex::Complex { re: {{_v[0-9]+}}.re / {{_v[0-9]+}}, im: {{_v[0-9]+}}.im / {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: __retval = num_complex::Complex { re: {{_v[0-9]+}}.re * {{_v[0-9]+}}, im: {{_v[0-9]+}}.im * {{_v[0-9]+}} };
// REWRITES-NEXT: coerce = __retval;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} = std::ptr::addr_of_mut!(coerce) as *mut {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT: return unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_arithmetic() {
// REWRITES-NEXT: let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut a: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut b: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce6: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut c: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce7: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce8: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce9: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce10: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce11: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce12: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce13: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce14: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: a = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]) };
// REWRITES-NEXT: b = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"add\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: coerce2 = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce2;
// REWRITES-NEXT: let mut byval: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"sub\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: coerce3 = num_complex::Complex { re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce3;
// REWRITES-NEXT: let mut byval2: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval2)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"mul\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} - {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} != {{_v[0-9]+}} { {{_v[0-9]+}} != {{_v[0-9]+}} } else { false };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: coerce4 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce4;
// REWRITES-NEXT: let mut byval3: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval3)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"div\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: coerce5 = __slate_cf80_div(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce5;
// REWRITES-NEXT: let mut byval4: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval4)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"neg\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: coerce6 = num_complex::Complex { re: -{{_v[0-9]+}}.re, im: -{{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce6;
// REWRITES-NEXT: let mut byval5: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval5)) });
// REWRITES-NEXT: c = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// REWRITES-NEXT: c = num_complex::Complex { re: {{_v[0-9]+}}.re + {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im + {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"add_assign\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce7 = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce7;
// REWRITES-NEXT: let mut byval6: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval6)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// REWRITES-NEXT: c = num_complex::Complex { re: {{_v[0-9]+}}.re - {{_v[0-9]+}}.re, im: {{_v[0-9]+}}.im - {{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"sub_assign\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce8 = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce8;
// REWRITES-NEXT: let mut byval7: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval7)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} - {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} != {{_v[0-9]+}} { {{_v[0-9]+}} != {{_v[0-9]+}} } else { false };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = __slate_cf80_mul(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: c = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"mul_assign\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce9 = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce9;
// REWRITES-NEXT: let mut byval8: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval8)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: c = __slate_cf80_div(num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} }, num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"div_assign\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce10 = c;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce10;
// REWRITES-NEXT: let mut byval9: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval9)) });
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !(a == a);
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = a == b;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !(a != b);
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"mix\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = b;
// REWRITES-NEXT: coerce11 = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce11;
// REWRITES-NEXT: coerce12 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce12;
// REWRITES-NEXT: let mut byval10: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let mut byval11: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce = mix_complex(unsafe { &(*std::ptr::addr_of_mut!(byval10)) }, unsafe { &(*std::ptr::addr_of_mut!(byval11)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce13 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce13;
// REWRITES-NEXT: let mut byval12: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval12)) });
// REWRITES-NEXT: a.re = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// REWRITES-NEXT: a.im = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"real_imag_assign\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce14 = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce14;
// REWRITES-NEXT: let mut byval13: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval13)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"real_field=%La imag_field=%La\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = a;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_casts() {
// REWRITES-NEXT: let mut ld: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut z: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut back: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut nonzero_imag: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut real_part: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut zd: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut coerce2: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut zf: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut coerce3: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut dd: f64 = 0.0;
// REWRITES-NEXT: let mut from_double: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut widened: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce4: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut i32: i32 = 0;
// REWRITES-NEXT: let mut fromi: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce5: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: ld = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT: z = num_complex::Complex { re: ld, im: LongDouble([0; 10]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"real_to_complex\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce;
// REWRITES-NEXT: let mut byval: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// REWRITES-NEXT: back = {{_v[0-9]+}}.re;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = back != LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: nonzero_imag = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = nonzero_imag;
// REWRITES-NEXT: real_part = {{_v[0-9]+}}.re;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = real_part != LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = nonzero_imag;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// REWRITES-NEXT: zd = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"to_double_complex\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<f64> = zd;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT: coerce2 = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce2;
// REWRITES-NEXT: let mut byval2: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval2)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = nonzero_imag;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// REWRITES-NEXT: zf = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"to_float_complex\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<f32> = zf;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-NEXT: coerce3 = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce3;
// REWRITES-NEXT: let mut byval3: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval3)) });
// REWRITES-NEXT: dd = 6.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT: from_double = num_complex::Complex { re: dd, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<f64> = from_double;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-NEXT: widened = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"from_double_complex\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce4 = widened;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce4;
// REWRITES-NEXT: let mut byval4: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval4)) });
// REWRITES-NEXT: i32 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32(i32);
// REWRITES-NEXT: fromi = num_complex::Complex { re: {{_v[0-9]+}}, im: LongDouble([0; 10]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"from_int\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce5 = fromi;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce5;
// REWRITES-NEXT: let mut byval5: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval5)) });
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_stdlib_functions() {
// REWRITES-NEXT: let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce3: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce4: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce5: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce6: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce7: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce8: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce9: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce10: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce11: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce12: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce13: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce14: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce15: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce16: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce17: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: LongDouble([0; 10]), __slate_anon_1: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut z: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce18: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce19: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce20: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce21: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce22: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce23: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce24: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce25: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce26: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce27: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce28: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce29: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce30: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce31: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce32: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce33: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce34: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce35: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce36: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce37: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce38: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce39: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce40: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce41: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce42: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce43: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce44: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce45: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce46: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce47: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce48: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce49: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce50: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce51: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce52: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce53: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce54: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut coerce55: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: z = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"cabs=%La\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce18 = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce18;
// REWRITES-NEXT: let mut byval: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = unsafe { __slate_cabsl__rf80_pcf80(std::ptr::addr_of_mut!(byval) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"carg=%La\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce19 = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce19;
// REWRITES-NEXT: let mut byval2: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = unsafe { __slate_cargl__rf80_pcf80(std::ptr::addr_of_mut!(byval2) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"conj\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// REWRITES-NEXT: coerce20 = num_complex::Complex { re: {{_v[0-9]+}}.re, im: -{{_v[0-9]+}}.im };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce20;
// REWRITES-NEXT: let mut byval3: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval3)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"cproj\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce21 = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce21;
// REWRITES-NEXT: let mut byval4: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce17 = unsafe { __slate_cprojl__rcf80_pcf80(std::ptr::addr_of_mut!(byval4) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce17) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce22 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce22;
// REWRITES-NEXT: let mut byval5: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval5)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"csqrt\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce23 = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce23;
// REWRITES-NEXT: let mut byval6: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce16 = unsafe { __slate_csqrtl__rcf80_pcf80(std::ptr::addr_of_mut!(byval6) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce16) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce24 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce24;
// REWRITES-NEXT: let mut byval7: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval7)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"cexp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce25 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce25;
// REWRITES-NEXT: let mut byval8: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce15 = unsafe { __slate_cexpl__rcf80_pcf80(std::ptr::addr_of_mut!(byval8) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce15) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce26 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce26;
// REWRITES-NEXT: let mut byval9: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval9)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"clog\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce27 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce27;
// REWRITES-NEXT: let mut byval10: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce14 = unsafe { __slate_clogl__rcf80_pcf80(std::ptr::addr_of_mut!(byval10) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce14) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce28 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce28;
// REWRITES-NEXT: let mut byval11: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval11)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"cpow\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: coerce29 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce29;
// REWRITES-NEXT: coerce30 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce30;
// REWRITES-NEXT: let mut byval12: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: let mut byval13: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce13 = unsafe { __slate_cpowl__rcf80_pcf80_pcf80(std::ptr::addr_of_mut!(byval12) as *mut num_complex::Complex<LongDouble>, std::ptr::addr_of_mut!(byval13) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce13) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce31 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce31;
// REWRITES-NEXT: let mut byval14: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval14)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"csin\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce32 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce32;
// REWRITES-NEXT: let mut byval15: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce12 = unsafe { __slate_csinl__rcf80_pcf80(std::ptr::addr_of_mut!(byval15) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce12) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce33 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce33;
// REWRITES-NEXT: let mut byval16: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval16)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ccos\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce34 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce34;
// REWRITES-NEXT: let mut byval17: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce11 = unsafe { __slate_ccosl__rcf80_pcf80(std::ptr::addr_of_mut!(byval17) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce11) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce35 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce35;
// REWRITES-NEXT: let mut byval18: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval18)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ctan\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce36 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce36;
// REWRITES-NEXT: let mut byval19: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce10 = unsafe { __slate_ctanl__rcf80_pcf80(std::ptr::addr_of_mut!(byval19) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce10) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce37 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce37;
// REWRITES-NEXT: let mut byval20: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval20)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"casin\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce38 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce38;
// REWRITES-NEXT: let mut byval21: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce9 = unsafe { __slate_casinl__rcf80_pcf80(std::ptr::addr_of_mut!(byval21) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce9) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce39 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce39;
// REWRITES-NEXT: let mut byval22: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval22)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"cacos\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce40 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce40;
// REWRITES-NEXT: let mut byval23: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce8 = unsafe { __slate_cacosl__rcf80_pcf80(std::ptr::addr_of_mut!(byval23) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce8) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce41 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce41;
// REWRITES-NEXT: let mut byval24: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval24)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"catan\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce42 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce42;
// REWRITES-NEXT: let mut byval25: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce7 = unsafe { __slate_catanl__rcf80_pcf80(std::ptr::addr_of_mut!(byval25) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce7) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce43 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce43;
// REWRITES-NEXT: let mut byval26: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval26)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"csinh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce44 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce44;
// REWRITES-NEXT: let mut byval27: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce6 = unsafe { __slate_csinhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval27) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce6) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce45 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce45;
// REWRITES-NEXT: let mut byval28: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval28)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ccosh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce46 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce46;
// REWRITES-NEXT: let mut byval29: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce5 = unsafe { __slate_ccoshl__rcf80_pcf80(std::ptr::addr_of_mut!(byval29) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce5) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce47 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce47;
// REWRITES-NEXT: let mut byval30: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval30)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ctanh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce48 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce48;
// REWRITES-NEXT: let mut byval31: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce4 = unsafe { __slate_ctanhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval31) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce4) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce49 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce49;
// REWRITES-NEXT: let mut byval32: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval32)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"casinh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce50 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce50;
// REWRITES-NEXT: let mut byval33: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce3 = unsafe { __slate_casinhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval33) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce3) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce51 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce51;
// REWRITES-NEXT: let mut byval34: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval34)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"cacosh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce52 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce52;
// REWRITES-NEXT: let mut byval35: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce2 = unsafe { __slate_cacoshl__rcf80_pcf80(std::ptr::addr_of_mut!(byval35) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce2) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce53 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce53;
// REWRITES-NEXT: let mut byval36: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval36)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"catanh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: coerce54 = num_complex::Complex { re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce54;
// REWRITES-NEXT: let mut byval37: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: coerce = unsafe { __slate_catanhl__rcf80_pcf80(std::ptr::addr_of_mut!(byval37) as *mut num_complex::Complex<LongDouble>) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut num_complex::Complex<LongDouble> = std::ptr::addr_of_mut!(coerce) as *mut num_complex::Complex<LongDouble>;
// REWRITES-NEXT: coerce55 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = coerce55;
// REWRITES-NEXT: let mut byval38: num_complex::Complex<LongDouble> = {{_v[0-9]+}};
// REWRITES-NEXT: print_lc({{_v[0-9]+}}, unsafe { &(*std::ptr::addr_of_mut!(byval38)) });
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"creal=%La cimag=%La\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// REWRITES-NEXT: let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = z;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_printf__ri32_pi8_f80_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: check_arithmetic();
// REWRITES-NEXT: check_casts();
// REWRITES-NEXT: check_stdlib_functions();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __slate_cabsl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn __slate_cacoshl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cacosl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cargl__rf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> LongDouble;
// REWRITES-NEXT:     fn __slate_casinhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_casinl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_catanhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_catanl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ccoshl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ccosl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cexpl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     safe fn __slate_cf80_div(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __slate_clogl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cpowl__rcf80_pcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>, _1: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_cprojl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_csinhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_csinl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_csqrtl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ctanhl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
// REWRITES-NEXT:     fn __slate_ctanl__rcf80_pcf80(_0: *mut num_complex::Complex<LongDouble>) -> {{anon_struct[0-9A-Za-z_]*}};
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
// REWRITES-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80_f80(_0: *mut i8, _1: *mut i8, _2: LongDouble, _3: LongDouble) -> i32;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
