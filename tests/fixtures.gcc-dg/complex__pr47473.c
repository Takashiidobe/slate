/* PR c/47473 */
/* { dg-do run } */
/* { dg-options "-std=c99" } */

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  long double _Complex w = 0.2L - 0.3iL;
  w                      = w * (0.3L - (0.0F + 1.0iF) * 0.9L);
  if (__builtin_fabsl(__real__ w + 0.21L) > 0.001L ||
      __builtin_fabsl(__imag__ w + 0.27L) > 0.001L)
    __builtin_abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-DAG:     let mut w: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:         re: LongDouble([205, 204, 204, 204, 204, 204, 204, 204, 252, 63])
// REWRITES-X86_64-GNU-DAG:             - LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:         im: LongDouble([154, 153, 153, 153, 153, 153, 153, 153, 253, 191]),
// REWRITES-AARCH64-GNU-DAG:     let mut w: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: 0.0f128,
// REWRITES-AARCH64-GNU-DAG:         im: 0.0f128,
// REWRITES-DAG:     };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = w;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([154, 153, 153, 153, 153, 153, 153, 153, 253, 63]);
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 2.000000e-01f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = -3.000000e-01f128;
// REWRITES-AARCH64-GNU-DAG:     w = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = w;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 3.000000e-01f128;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-DAG:         re: 0.0 + {{__v[0-9]+}},
// REWRITES-DAG:         im: {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-DAG:         im: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:         re: {{__v[0-9]+}}.re as f128,
// REWRITES-AARCH64-GNU-DAG:         im: {{__v[0-9]+}}.im as f128,
// REWRITES-DAG:     };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([102, 102, 102, 102, 102, 102, 102, 230, 254, 63]);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 9.000000e-01f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-DAG:         re: {{__v[0-9]+}} - {{__v[0-9]+}}.re * {{__v[0-9]+}},
// REWRITES-DAG:         im: -({{__v[0-9]+}}.im * {{__v[0-9]+}}),
// REWRITES-DAG:     };
// REWRITES-X86_64-GNU-DAG:     w = unsafe {
// REWRITES-X86_64-GNU-DAG:         __slate_cf80_mul(
// REWRITES-X86_64-GNU-DAG:             num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:             },
// REWRITES-X86_64-GNU-DAG:             num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: {{__v[0-9]+}}.re,
// REWRITES-X86_64-GNU-DAG:                 im: {{__v[0-9]+}}.im,
// REWRITES-X86_64-GNU-DAG:             },
// REWRITES-X86_64-GNU-DAG:         )
// REWRITES-X86_64-GNU-DAG:     };
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = w;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble =
// REWRITES-X86_64-GNU-DAG:         __slate_f80_abs({{__v[0-9]+}}.re + LongDouble([61, 10, 215, 163, 112, 61, 10, 215, 252, 63]));
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > LongDouble([59, 223, 79, 141, 151, 110, 18, 131, 245, 63]);
// REWRITES-AARCH64-GNU-DAG:     w = unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = w;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 2.100000e-01f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.000000e-03f128;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = w;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble =
// REWRITES-X86_64-GNU-DAG:             __slate_f80_abs({{__v[0-9]+}}.im + LongDouble([113, 61, 10, 215, 163, 112, 61, 138, 253, 63]));
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > LongDouble([59, 223, 79, 141, 151, 110, 18, 131, 245, 63]);
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = w;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 2.700000e-01f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 1.000000e-03f128;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-DAG:     let mut w: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:         re: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-DAG:         im: LongDouble([0; 10]),
// LOWERING-AARCH64-GNU-DAG:     let mut w: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:         re: 0.0f128,
// LOWERING-AARCH64-GNU-DAG:         im: 0.0f128,
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 204, 252, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([154, 153, 153, 153, 153, 153, 153, 153, 253, 191]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 2.000000e-01f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = -3.000000e-01f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-DAG:     w = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = w;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([154, 153, 153, 153, 153, 153, 153, 153, 253, 63]);
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = w;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 3.000000e-01f128;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 0.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 1.0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re as f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im as f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([102, 102, 102, 102, 102, 102, 102, 230, 254, 63]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = -{{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 9.000000e-01f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = -{{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:         re: {{__v[0-9]+}},
// LOWERING-DAG:         im: {{__v[0-9]+}},
// LOWERING-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = unsafe {
// LOWERING-X86_64-GNU-DAG:         __slate_cf80_mul(
// LOWERING-X86_64-GNU-DAG:             num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:             },
// LOWERING-X86_64-GNU-DAG:             num_complex::Complex {
// LOWERING-X86_64-GNU-DAG:                 re: {{__v[0-9]+}}.re,
// LOWERING-X86_64-GNU-DAG:                 im: {{__v[0-9]+}}.im,
// LOWERING-X86_64-GNU-DAG:             },
// LOWERING-X86_64-GNU-DAG:         )
// LOWERING-X86_64-GNU-DAG:     };
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-AARCH64-GNU-DAG:         unsafe { __multc3({{__v[0-9]+}}.re, {{__v[0-9]+}}.im, {{__v[0-9]+}}.re, {{__v[0-9]+}}.im) };
// LOWERING-DAG:     w = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = w;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([61, 10, 215, 163, 112, 61, 10, 215, 252, 63]);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([59, 223, 79, 141, 151, 110, 18, 131, 245, 63]);
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: num_complex::Complex<f128> = w;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 2.100000e-01f128;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: f128 = 1.000000e-03f128;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = w;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([113, 61, 10, 215, 163, 112, 61, 138, 253, 63]);
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([59, 223, 79, 141, 151, 110, 18, 131, 245, 63]);
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = w;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 2.700000e-01f128;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.abs();
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 1.000000e-03f128;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering
