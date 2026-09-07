/* Test __builtin_complex semantics.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */
/* { dg-require-effective-target inf } */
/* { dg-add-options ieee } */
/* { dg-skip-if "double support is incomplete" { "avr-*-*" } } */

extern void exit(int);
extern void abort(void);

#define COMPARE_BODY(A, B, TYPE, COPYSIGN)                                     \
  do {                                                                         \
    TYPE s1 = COPYSIGN((TYPE)1.0, A);                                          \
    TYPE s2 = COPYSIGN((TYPE)1.0, B);                                          \
    if (s1 != s2)                                                              \
      abort();                                                                 \
    if ((__builtin_isnan(A) != 0) != (__builtin_isnan(B) != 0))                \
      abort();                                                                 \
    if ((A != B) != (__builtin_isnan(A) != 0))                                 \
      abort();                                                                 \
  } while (0)

void comparef(float a, float b) {
  COMPARE_BODY(a, b, float, __builtin_copysignf);
}

void compare(double a, double b) {
  COMPARE_BODY(a, b, double, __builtin_copysign);
}

void comparel(long double a, long double b) {
  COMPARE_BODY(a, b, long double, __builtin_copysignl);
}

void comparecf(_Complex float a, float r, float i) {
  comparef(__real__ a, r);
  comparef(__imag__ a, i);
}

void comparec(_Complex double a, double r, double i) {
  compare(__real__ a, r);
  compare(__imag__ a, i);
}

void comparecl(_Complex long double a, long double r, long double i) {
  comparel(__real__ a, r);
  comparel(__imag__ a, i);
}

#define VERIFY(A, B, TYPE, COMPARE)                                            \
  do {                                                                         \
    TYPE                 a  = A;                                               \
    TYPE                 b  = B;                                               \
    _Complex TYPE        cr = __builtin_complex(a, b);                         \
    static _Complex TYPE cs = __builtin_complex(A, B);                         \
    COMPARE(cr, A, B);                                                         \
    COMPARE(cs, A, B);                                                         \
  } while (0)

#define ALL_CHECKS(PZ, NZ, NAN, INF, TYPE, COMPARE)                            \
  do {                                                                         \
    VERIFY(PZ, PZ, TYPE, COMPARE);                                             \
    VERIFY(PZ, NZ, TYPE, COMPARE);                                             \
    VERIFY(PZ, NAN, TYPE, COMPARE);                                            \
    VERIFY(PZ, INF, TYPE, COMPARE);                                            \
    VERIFY(NZ, PZ, TYPE, COMPARE);                                             \
    VERIFY(NZ, NZ, TYPE, COMPARE);                                             \
    VERIFY(NZ, NAN, TYPE, COMPARE);                                            \
    VERIFY(NZ, INF, TYPE, COMPARE);                                            \
    VERIFY(NAN, PZ, TYPE, COMPARE);                                            \
    VERIFY(NAN, NZ, TYPE, COMPARE);                                            \
    VERIFY(NAN, NAN, TYPE, COMPARE);                                           \
    VERIFY(NAN, INF, TYPE, COMPARE);                                           \
    VERIFY(INF, PZ, TYPE, COMPARE);                                            \
    VERIFY(INF, NZ, TYPE, COMPARE);                                            \
    VERIFY(INF, NAN, TYPE, COMPARE);                                           \
    VERIFY(INF, INF, TYPE, COMPARE);                                           \
  } while (0)

void check_float(void) {
  ALL_CHECKS(0.0f, -0.0f, __builtin_nanf(""), __builtin_inff(), float,
             comparecf);
}

void check_double(void) {
  ALL_CHECKS(0.0, -0.0, __builtin_nan(""), __builtin_inf(), double, comparec);
}

// @lowering-fn-begin
// @rewrite-fn-begin
void check_long_double(void) {
  ALL_CHECKS(0.0l, -0.0l, __builtin_nanl(""), __builtin_infl(), long double,
             comparecl);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  check_float();
  check_double();
  check_long_double();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn check_long_double() {
// LOWERING-DAG:     {
// LOWERING-DAG:         loop {
// LOWERING-DAG:             {
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-X86_64-GNU-DAG:                                 unsafe { check_long_double_cs };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs };
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_1 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_2 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_3 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_4 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_5 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_6 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_7 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_8 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_9 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_10 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_11 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_12 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_13 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_14 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         {
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-DAG:                                 re: {{__v[0-9]+}},
// LOWERING-DAG:                                 im: {{__v[0-9]+}},
// LOWERING-DAG:                             };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> =
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: num_complex::Complex<f128> =
// LOWERING-DAG:                                 unsafe { check_long_double_cs_15 };
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-X86_64-GNU-DAG:                             let {{__v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-DAG:                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-AARCH64-GNU-DAG:                             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// LOWERING-DAG:                             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     return;
// LOWERING-DAG: }
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     check_float();
// LOWERING-DAG:     check_double();
// LOWERING-DAG:     check_long_double();
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn check_long_double() {
// REWRITES-DAG:     loop {
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex { re: {{__v[0-9]+}}, im: {{__v[0-9]+}} };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_1 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_1 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_2 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_2 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_3 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_3 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_4 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_4 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_5 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_5 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_6 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_6 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_7 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_7 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_8 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_8 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_9 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_9 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_10 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_10 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_11 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_11 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_12 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_12 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_13 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_13 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = -0.000000e+00f128;
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_14 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_14 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff8000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         loop {
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-DAG:                 re: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-X86_64-GNU-DAG:                 im: LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]),
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-DAG:                 re: {{__v[0-9]+}},
// REWRITES-AARCH64-GNU-DAG:                 im: {{__v[0-9]+}},
// REWRITES-DAG:             };
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-X86_64-GNU-DAG:             comparecl(unsafe { check_long_double_cs_15 }, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: num_complex::Complex<f128> = unsafe { check_long_double_cs_15 };
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: f128 = f128::from_bits(0x7fff0000000000000000000000000000);
// REWRITES-AARCH64-GNU-DAG:             comparecl({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         if !({{__v[0-9]+}} != 0) {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     return;
// REWRITES-DAG: }
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     check_float();
// REWRITES-DAG:     check_double();
// REWRITES-DAG:     check_long_double();
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
