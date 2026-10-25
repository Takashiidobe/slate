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

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn cacosl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn casinl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn catanl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn ccosl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn csinl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn ctanl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn cacoshl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn casinhl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn catanhl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn ccoshl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn csinhl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn ctanhl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn cexpl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn clogl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn cpowl(
// CHECK-NEXT:         __x: num_complex::Complex<f128>,
// CHECK-NEXT:         __y: num_complex::Complex<f128>,
// CHECK-NEXT:     ) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn csqrtl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn cabsl(__z: num_complex::Complex<f128>) -> f128;
// CHECK-NEXT:     fn cargl(__z: num_complex::Complex<f128>) -> f128;
// CHECK-NEXT:     fn conjl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn cprojl(__z: num_complex::Complex<f128>) -> num_complex::Complex<f128>;
// CHECK-NEXT:     fn cimagl(__z: num_complex::Complex<f128>) -> f128;
// CHECK-NEXT:     fn creall(__z: num_complex::Complex<f128>) -> f128;
// CHECK-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn print_lc(name: *const u8, z: num_complex::Complex<f128>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s=%Lax%Lai\n".as_ptr(), name, creall(z), cimagl(z)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn mix_complex(
// CHECK-NEXT:     a: num_complex::Complex<f128>,
// CHECK-NEXT:     b: num_complex::Complex<f128>,
// CHECK-NEXT: ) -> num_complex::Complex<f128> {
// CHECK-NEXT:     let c: num_complex::Complex<f128> = (a + b) / 2f128;
// CHECK-NEXT:     c * 3f128
// CHECK-NEXT: }
// CHECK-NEXT: fn check_arithmetic() {
// CHECK-NEXT:     let mut a: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: 1f128,
// CHECK-NEXT:         im: 2f128,
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: 3f128,
// CHECK-NEXT:         im: -1f128,
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lc(c"add".as_ptr(), a + b);
// CHECK-NEXT:     print_lc(c"sub".as_ptr(), a - b);
// CHECK-NEXT:     print_lc(c"mul".as_ptr(), a.c_mul(b, false));
// CHECK-NEXT:     print_lc(c"div".as_ptr(), a.c_div(b, false));
// CHECK-NEXT:     print_lc(c"neg".as_ptr(), -a);
// CHECK-NEXT:     let mut c: num_complex::Complex<f128> = a;
// CHECK-NEXT:     c += b;
// CHECK-NEXT:     print_lc(c"add_assign".as_ptr(), c);
// CHECK-NEXT:     c -= b;
// CHECK-NEXT:     print_lc(c"sub_assign".as_ptr(), c);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: num_complex::Complex<f128> = c;
// CHECK-NEXT:     c = [[V_0]].c_mul(b, false);
// CHECK-NEXT:     print_lc(c"mul_assign".as_ptr(), c);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: num_complex::Complex<f128> = c;
// CHECK-NEXT:     c = [[V_1]].c_div(b, false);
// CHECK-NEXT:     print_lc(c"div_assign".as_ptr(), c);
// CHECK-NEXT:     if a != a {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if a == b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if a == b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     print_lc(c"mix".as_ptr(), mix_complex(a, b));
// CHECK-NEXT:     a.re = 9f128;
// CHECK-NEXT:     a.im = 8f128;
// CHECK-NEXT:     print_lc(c"real_imag_assign".as_ptr(), a);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"real_field=%La imag_field=%La\n".as_ptr(), a.re, a.im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn check_casts() {
// CHECK-NEXT:     let ld: f128 = 5f128;
// CHECK-NEXT:     let z: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: ld,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lc(c"real_to_complex".as_ptr(), z);
// CHECK-NEXT:     let back: f128 = z.re;
// CHECK-NEXT:     if back != 5f128 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let nonzero_imag: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: 3f128,
// CHECK-NEXT:         im: 4f128,
// CHECK-NEXT:     };
// CHECK-NEXT:     let real_part: f128 = nonzero_imag.re;
// CHECK-NEXT:     if real_part != 3f128 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let zd: num_complex::Complex<f64> = {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = nonzero_imag;
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re as f64,
// CHECK-NEXT:             im: [[T_0]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"to_double_complex".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = zd;
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]].re as f128,
// CHECK-NEXT:                 im: [[T_1]].im as f128,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     let zf: num_complex::Complex<f32> = {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = nonzero_imag;
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_2]].re as f32,
// CHECK-NEXT:             im: [[T_2]].im as f32,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"to_float_complex".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = zf;
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_3]].re as f128,
// CHECK-NEXT:                 im: [[T_3]].im as f128,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     let dd: f64 = 6.0f64;
// CHECK-NEXT:     let from_double: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: dd,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     let widened: num_complex::Complex<f128> = {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = from_double;
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_4]].re as f128,
// CHECK-NEXT:             im: [[T_4]].im as f128,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lc(c"from_double_complex".as_ptr(), widened);
// CHECK-NEXT:     let i32: i32 = 7;
// CHECK-NEXT:     let fromi: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: i32 as f128,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lc(c"from_int".as_ptr(), fromi);
// CHECK-NEXT: }
// CHECK-NEXT: fn check_stdlib_functions() {
// CHECK-NEXT:     let z: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: 3f128,
// CHECK-NEXT:         im: 4f128,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"cabs=%La\n".as_ptr(), cabsl(z)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"carg=%La\n".as_ptr(), cargl(z)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     print_lc(c"conj".as_ptr(), unsafe { conjl(z) });
// CHECK-NEXT:     print_lc(c"cproj".as_ptr(), unsafe { cprojl(z) });
// CHECK-NEXT:     print_lc(c"csqrt".as_ptr(), unsafe { csqrtl(z) });
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"cexp".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cexpl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"clog".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             clogl(num_complex::Complex {
// CHECK-NEXT:                 re: 1f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"cpow".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cpowl(
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: 2f128,
// CHECK-NEXT:                     im: 0f128,
// CHECK-NEXT:                 },
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: 3f128,
// CHECK-NEXT:                     im: 0f128,
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"csin".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             csinl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"ccos".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ccosl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"ctan".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ctanl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"casin".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             casinl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"cacos".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cacosl(num_complex::Complex {
// CHECK-NEXT:                 re: 1f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"catan".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             catanl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"csinh".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             csinhl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"ccosh".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ccoshl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"ctanh".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ctanhl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"casinh".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             casinhl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"cacosh".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cacoshl(num_complex::Complex {
// CHECK-NEXT:                 re: 1f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     print_lc(
// CHECK-NEXT:         c"catanh".as_ptr(),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             catanhl(num_complex::Complex {
// CHECK-NEXT:                 re: 0f128,
// CHECK-NEXT:                 im: 0f128,
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"creal=%La cimag=%La\n".as_ptr(), creall(z), cimagl(z)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check_arithmetic();
// CHECK-NEXT:     check_casts();
// CHECK-NEXT:     check_stdlib_functions();
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
