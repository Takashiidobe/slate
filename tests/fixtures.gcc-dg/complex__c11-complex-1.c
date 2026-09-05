/* Test complex divide does not have the bug identified in N1496.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */
/* { dg-add-options ieee } */

extern void abort(void);
extern void exit(int);

#define CMPLX(x, y)  __builtin_complex((double)(x), (double)(y))
#define CMPLXF(x, y) __builtin_complex((float)(x), (float)(y))
#define CMPLXL(x, y) __builtin_complex((long double)(x), (long double)(y))
#define NAN          __builtin_nanf("")
#define isnan(x)     __builtin_isnan(x)

volatile _Complex float num_f = CMPLXF(1, 1);
volatile _Complex float den_f = CMPLXF(0, NAN);
volatile _Complex float res_f, cres_f = CMPLXF(1, 1) / CMPLXF(0, NAN);

volatile _Complex double num_d = CMPLX(1, 1);
volatile _Complex double den_d = CMPLX(0, NAN);
volatile _Complex double res_d, cres_d = CMPLX(1, 1) / CMPLX(0, NAN);

volatile _Complex long double num_ld = CMPLXL(1, 1);
volatile _Complex long double den_ld = CMPLXL(0, NAN);
volatile _Complex long double res_ld, cres_ld = CMPLXL(1, 1) / CMPLXL(0, NAN);

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  res_f = num_f / den_f;
  if (!isnan(__real__ res_f) || !isnan(__imag__ res_f) ||
      !isnan(__real__ cres_f) || !isnan(__imag__ cres_f))
    abort();
  res_d = num_d / den_d;
  if (!isnan(__real__ res_d) || !isnan(__imag__ res_d) ||
      !isnan(__real__ cres_d) || !isnan(__imag__ cres_d))
    abort();
  res_ld = num_ld / den_ld;
  if (!isnan(__real__ res_ld) || !isnan(__imag__ res_ld) ||
      !isnan(__real__ cres_ld) || !isnan(__imag__ cres_ld))
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num_f)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den_f)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { __divsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(res_f), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_f)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_f)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_f)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f32> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_f)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num_d)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den_d)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:         unsafe { __divdc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(res_d), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_d)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_d)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_d)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_d)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num_ld)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den_ld)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-DAG:         __slate_cf80_div(
// LOWERING-DAG:             num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}}.re,
// LOWERING-DAG:                 im: {{__v[0-9]+}}.im,
// LOWERING-DAG:             },
// LOWERING-DAG:             num_complex::Complex {
// LOWERING-DAG:                 re: {{__v[0-9]+}}.re,
// LOWERING-DAG:                 im: {{__v[0-9]+}}.im,
// LOWERING-DAG:             },
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(res_ld), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_ld)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_ld)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_ld)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-DAG:                 unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_ld)) };
// LOWERING-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num_f)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den_f)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = unsafe { __divsc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(res_f), {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_f)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_f)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_f)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f32> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_f)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num_d)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den_d)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:         unsafe { __divdc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(res_d), {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_d)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_d)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_d)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_d)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(num_ld)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(den_ld)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// REWRITES-DAG:         __slate_cf80_div(
// REWRITES-DAG:             num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}}.re,
// REWRITES-DAG:                 im: {{__v[0-9]+}}.im,
// REWRITES-DAG:             },
// REWRITES-DAG:             num_complex::Complex {
// REWRITES-DAG:                 re: {{__v[0-9]+}}.re,
// REWRITES-DAG:                 im: {{__v[0-9]+}}.im,
// REWRITES-DAG:             },
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(res_ld), {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-DAG:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_ld)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(res_ld)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_ld)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// REWRITES-DAG:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(cres_ld)) };
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
