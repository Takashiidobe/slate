/* Verify that rtl expansion cleanup doesn't get too aggressive about
   code dealing with complex CONCATs.  */
/* { dg-do run } */
/* { dg-options "-O -fno-tree-sra" } */

extern void abort(void);
extern void exit(int);

__complex__ float foo(void) {
  __complex__ float f[1];
  __real__ f[0] = 1;
  __imag__ f[0] = 1;
  f[0]          = __builtin_conjf(f[0]);
  return f[0];
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  __complex__ double d[1];
  d[0] = foo();
  if (__real__ d[0] != 1 || __imag__ d[0] != -1)
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut d: aligned::Aligned<aligned::A16, [num_complex::Complex<f64>; 1]> =
// LOWERING-DAG:         aligned::Aligned([num_complex::Complex { re: 0.0, im: 0.0 }; 1]);
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = foo();
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im as f64;
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     d[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = d[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f64> = d[({{__v[0-9]+}} as usize)];
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
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
// REWRITES-DAG:     let mut d: aligned::Aligned<aligned::A16, [num_complex::Complex<f64>; 1]> =
// REWRITES-DAG:         aligned::Aligned([num_complex::Complex { re: 0.0, im: 0.0 }; 1]);
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = foo();
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     d[({{__v[0-9]+}} as usize)] = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}}.re as f64,
// REWRITES-DAG:         im: {{__v[0-9]+}}.im as f64,
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = d[0];
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}}.re != ((1 as i32) as f64) {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f64> = d[0];
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.im != ((-1 as i32) as f64);
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
