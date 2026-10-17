#include <stdio.h>

typedef _Complex long double cl;

static cl global_cl = 1.25L;

static cl make(long double re, long double im) {
  cl z = re;
  __imag__ z = im;
  return z;
}

static cl mul(cl a, cl b) { return a * b; }
static cl div(cl a, cl b) { return a / b; }

static void show(const char *name, cl z) {
  printf("%s %a %a\n", name, (double)__real__ z, (double)__imag__ z);
}

int main(void) {
  cl a = make(1.5L, -2.25L);
  cl b = make(0.5L, 4.0L);
  show("add", a + b);
  show("sub", a - b);
  show("mul", mul(a, b));
  show("div", div(a, b));
  show("neg", -a);
  show("conj", ~a);
  show("scale", a * 2.0L);
  show("rdiv", 3.0L / b);
  long double inf = __builtin_infl();
  show("infmul", mul(make(inf, __builtin_nanl("")), make(1.0L, 1.0L)));
  show("divzero", div(make(1.0L, 1.0L), make(0.0L, 0.0L)));
  cl tiny = make(0x1p-16000L, 0x1p-16000L);
  show("tinydiv", div(tiny, tiny));
  long double precise = __real__ mul(make(1.0L + 0x1p-100L, 0), make(1.0L - 0x1p-100L, 0));
  printf("%d %d %d\n", a == a, a == 1.5L, precise == 1.0L);
  cl negzero = make(1.0L, -0.0L);
  show("radd", 2.0L + negzero);
  show("rsub", 2.0L - negzero);
  show("rmul", negzero * 3.0L);
  show("rdivz", negzero / 4.0L);
  double _Complex narrowed = a;
  printf("narrow %a %a\n", __real__ narrowed, __imag__ narrowed);
  float _Complex f = 0.25f;
  __imag__ f = 8.0f;
  show("widen", f);
  show("int", 7);
  cl acc = a;
  acc += b;
  acc *= b;
  acc -= 1;
  acc /= 2.0L;
  show("acc", acc);
  cl *p = &acc;
  __real__ *p = 9.0L;
  show("place", *p);
  show("global", global_cl);
  printf("%d %d %d\n", !make(0, 0), !!a, a ? 1 : 0);
  long double re = a;
  int truncated = a;
  printf("%a %d\n", (double)re, truncated);
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
// CHECK-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut global_cl: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:     re: 1.25f128,
// CHECK-NEXT:     im: 0.0,
// CHECK-NEXT: };
// CHECK-NEXT: fn make(re: f128, im: f128) -> num_complex::Complex<f128> {
// CHECK-NEXT:     let mut z: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: re,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     z.im = im;
// CHECK-NEXT:     z
// CHECK-NEXT: }
// CHECK-NEXT: fn mul(
// CHECK-NEXT:     mut a: num_complex::Complex<f128>,
// CHECK-NEXT:     b: num_complex::Complex<f128>,
// CHECK-NEXT: ) -> num_complex::Complex<f128> {
// CHECK-NEXT:     a.c_mul(b, false)
// CHECK-NEXT: }
// CHECK-NEXT: fn div(
// CHECK-NEXT:     mut a: num_complex::Complex<f128>,
// CHECK-NEXT:     b: num_complex::Complex<f128>,
// CHECK-NEXT: ) -> num_complex::Complex<f128> {
// CHECK-NEXT:     a.c_div(b, false)
// CHECK-NEXT: }
// CHECK-NEXT: fn show(name: *const u8, z: num_complex::Complex<f128>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %a %a\n".as_ptr(), name, z.re as f64, z.im as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: num_complex::Complex<f128> = make(1.5f128, -2.25f128);
// CHECK-NEXT:     let b: num_complex::Complex<f128> = make(0.5f128, 4f128);
// CHECK-NEXT:     show(c"add".as_ptr(), a + b);
// CHECK-NEXT:     show(c"sub".as_ptr(), a - b);
// CHECK-NEXT:     show(c"mul".as_ptr(), mul(a, b));
// CHECK-NEXT:     show(c"div".as_ptr(), div(a, b));
// CHECK-NEXT:     show(c"neg".as_ptr(), -a);
// CHECK-NEXT:     show(c"conj".as_ptr(), a.conj());
// CHECK-NEXT:     show(c"scale".as_ptr(), a * 2f128);
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"rdiv".as_ptr(),
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: 3f128,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         }
// CHECK-NEXT:             .c_div(b, false),
// CHECK-NEXT:     );
// CHECK-NEXT:     let inf: f128 = f128::INFINITY;
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"infmul".as_ptr(),
// CHECK-NEXT:         mul(
// CHECK-NEXT:             make(inf, f128::from_bits(0x7fff8000000000000000000000000000)),
// CHECK-NEXT:             make(1f128, 1f128),
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(c"divzero".as_ptr(), div(make(1f128, 1f128), make(0f128, 0f128)));
// CHECK-NEXT:     let tiny: num_complex::Complex<f128> = make(
// CHECK-NEXT:         3.31184022194550157139472849083578569E-4817f128,
// CHECK-NEXT:         3.31184022194550157139472849083578569E-4817f128,
// CHECK-NEXT:     );
// CHECK-NEXT:     show(c"tinydiv".as_ptr(), div(tiny, tiny));
// CHECK-NEXT:     let precise: f128 = mul(
// CHECK-NEXT:             make(
// CHECK-NEXT:                 1f128 + 7.88860905221011805411728565282786229E-31f128,
// CHECK-NEXT:                 (0 as i32) as f128,
// CHECK-NEXT:             ),
// CHECK-NEXT:             make(
// CHECK-NEXT:                 1f128 - 7.88860905221011805411728565282786229E-31f128,
// CHECK-NEXT:                 (0 as i32) as f128,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:         .re;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (a == a) as i32, (a == num_complex::Complex { re : 1.5f128, im : 0.0
// CHECK-NEXT:         }) as i32, (precise == 1f128) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let negzero: num_complex::Complex<f128> = make(1f128, -0f128);
// CHECK-NEXT:     show(c"radd".as_ptr(), 2f128 + negzero);
// CHECK-NEXT:     show(c"rsub".as_ptr(), 2f128 - negzero);
// CHECK-NEXT:     show(c"rmul".as_ptr(), negzero * 3f128);
// CHECK-NEXT:     show(c"rdivz".as_ptr(), negzero / 4f128);
// CHECK-NEXT:     let narrowed: num_complex::Complex<f64> = {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = a;
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: [[T_0]].re as f64,
// CHECK-NEXT:             im: [[T_0]].im as f64,
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"narrow %a %a\n".as_ptr(), narrowed.re, narrowed.im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut f: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:         re: 0.25f32,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     };
// CHECK-NEXT:     f.im = 8.0f32;
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"widen".as_ptr(),
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = f;
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]].re as f128,
// CHECK-NEXT:                 im: [[T_1]].im as f128,
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"int".as_ptr(),
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: (7 as i32) as f128,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut acc: num_complex::Complex<f128> = a;
// CHECK-NEXT:     acc += b;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: num_complex::Complex<f128> = acc;
// CHECK-NEXT:     acc = [[V_0]].c_mul(b, false);
// CHECK-NEXT:     acc -= (1 as i32) as f128;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: num_complex::Complex<f128> = acc / 2f128;
// CHECK-NEXT:     acc = [[V_1]];
// CHECK-NEXT:     show(c"acc".as_ptr(), acc);
// CHECK-NEXT:     let p: *mut num_complex::Complex<f128> = std::ptr::addr_of_mut!(acc);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).re = 9f128;
// CHECK-NEXT:     }
// CHECK-NEXT:     show(c"place".as_ptr(), unsafe { *p });
// CHECK-NEXT:     show(c"global".as_ptr(), unsafe { global_cl });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", ! (make((0 as i32) as f128, (0 as i32) as f128) !=
// CHECK-NEXT:         num_complex::Complex { re : 0f128, im : 0.0 }) as i32, ! (! (a !=
// CHECK-NEXT:         num_complex::Complex { re : 0f128, im : 0.0 })) as i32, if (a !=
// CHECK-NEXT:         num_complex::Complex { re : 0f128, im : 0.0 }) { 1 as i32 } else { 0 as i32 }
// CHECK-NEXT:     );
// CHECK-NEXT:     let re: f128 = a.re;
// CHECK-NEXT:     let truncated: i32 = a.re as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%a %d\n".as_ptr(), re as f64, truncated) };
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
