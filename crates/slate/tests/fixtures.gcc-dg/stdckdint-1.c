/* Test C23 Checked Integer Arithmetic macros in <stdckdint.h>.  */
/* { dg-do run } */
/* { dg-options "-std=c23" } */

#include <stdckdint.h>

#if __STDC_VERSION_STDCKDINT_H__ != 202311L
# error __STDC_VERSION_STDCKDINT_H__ not defined to 202311L
#endif

extern void abort (void);

// @lowering-fn-begin
// @rewrite-fn-begin
int
main ()
{
  unsigned int a;
  if (ckd_add (&a, 1, 2) || a != 3)
    abort ();
  if (ckd_add (&a, ~2U, 2) || a != ~0U)
    abort ();
  if (!ckd_add (&a, ~2U, 4) || a != 1)
    abort ();
  if (ckd_sub (&a, 42, 2) || a != 40)
    abort ();
  if (!ckd_sub (&a, 11, ~0ULL) || a != 12)
    abort ();
  if (ckd_mul (&a, 42, 16U) || a != 672)
    abort ();
  if (ckd_mul (&a, ~0UL, 0) || a != 0)
    abort ();
  if (ckd_mul (&a, 1, ~0U) || a != ~0U)
    abort ();
  if (ckd_mul (&a, ~0UL, 1) != (~0UL > ~0U) || a != ~0U)
    abort ();
  static_assert (_Generic (ckd_add (&a, 1, 1), bool: 1, default: 0));
  static_assert (_Generic (ckd_sub (&a, 1, 1), bool: 1, default: 0));
  static_assert (_Generic (ckd_mul (&a, 1, 1), bool: 1, default: 0));
  signed char b;
  if (ckd_add (&b, 8, 12) || b != 20)
    abort ();
  if (ckd_sub (&b, 8UL, 12ULL) || b != -4)
    abort ();
  if (ckd_mul (&b, 2, 3) || b != 6)
    abort ();
  unsigned char c;
  if (ckd_add (&c, 8, 12) || c != 20)
    abort ();
  if (ckd_sub (&c, 8UL, 12ULL) != (-4ULL > (unsigned char) -4U)
      || c != (unsigned char) -4U)
    abort ();
  if (ckd_mul (&c, 2, 3) || c != 6)
    abort ();
  long long d;
  if (ckd_add (&d, ~0U, ~0U) != (~0U + 1ULL < ~0U)
      || d != (long long) (2 * (unsigned long long) ~0U))
    abort ();
  if (ckd_sub (&d, 0, 0) || d != 0)
    abort ();
  if (ckd_mul (&d, 16, 1) || d != 16)
    abort ();
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut a: u32 = 0;
// LOWERING-DAG:     let mut b: i8 = 0;
// LOWERING-DAG:     let mut c: u8 = 0;
// LOWERING-DAG:     let mut d: i64 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("1");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("2");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_add({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 3;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4294967293");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("2");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_add({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4294967293");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_add({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 1;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("42");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("2");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_sub({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 40;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("11");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> =
// LOWERING-DAG:             bitint::BInt::<65, 2, 16>::from_decimal_str("18446744073709551615");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_sub({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 12;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("42");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("16");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 672;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> =
// LOWERING-DAG:             bitint::BInt::<65, 2, 16>::from_decimal_str("18446744073709551615");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("0");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 0;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("1");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4294967295");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> =
// LOWERING-DAG:             bitint::BInt::<65, 2, 16>::from_decimal_str("18446744073709551615");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("1");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// LOWERING-DAG:         a = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 18446744073709551615u64;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 4294967295u64;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = a;
// LOWERING-DAG:             let {{_v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 12;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.0 as i8;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -128 || {{_v[0-9]+}}.0 > 127);
// LOWERING-DAG:         b = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: i8 = b;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = 20;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("8");
// LOWERING-DAG:         let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("12");
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_sub({{_v[0-9]+}}.to_i128());
// LOWERING-DAG:         let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.0 as i8;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -128 || {{_v[0-9]+}}.0 > 127);
// LOWERING-DAG:         b = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: i8 = b;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = -4;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i8 = {{_v[0-9]+}}.0 as i8;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -128 || {{_v[0-9]+}}.0 > 127);
// LOWERING-DAG:         b = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: i8 = b;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = 6;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 12;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: u8 = {{_v[0-9]+}}.0 as u8;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 255);
// LOWERING-DAG:         c = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u8 = c;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = 20;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 12;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: u8 = {{_v[0-9]+}}.0 as u8;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 255;
// LOWERING-DAG:         c = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 18446744073709551612u64;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 252;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u8 = c;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = 252;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: u8 = {{_v[0-9]+}}.0 as u8;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 255);
// LOWERING-DAG:         c = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: u8 = c;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = 6;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = 4294967295i64;
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = 4294967295i64;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}}.0 as i64;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool =
// LOWERING-DAG:             {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// LOWERING-DAG:         d = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 4294967295u64;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: u64 = 4294967295u64;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = d;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 2;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 4294967295u64;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}}.0 as i64;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool =
// LOWERING-DAG:             {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// LOWERING-DAG:         d = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = d;
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = 0;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = 16;
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}}.0 as i64;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool =
// LOWERING-DAG:             {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// LOWERING-DAG:         d = {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = d;
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = 16;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:             {{_v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut a: u32 = 0;
// REWRITES-DAG:     let mut b: i8 = 0;
// REWRITES-DAG:     let mut c: u8 = 0;
// REWRITES-DAG:     let mut d: i64 = 0;
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("1");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("2");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_add({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 3;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4294967293");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("2");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_add({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 4294967295u32;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4294967293");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_add({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 1;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("42");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("2");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_sub({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 40;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("11");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> =
// REWRITES-DAG:         bitint::BInt::<65, 2, 16>::from_decimal_str("18446744073709551615");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_sub({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 12;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("42");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("16");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 672;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> =
// REWRITES-DAG:         bitint::BInt::<65, 2, 16>::from_decimal_str("18446744073709551615");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("0");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 0;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("1");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<33, 1, 8> = bitint::BInt::<33, 1, 8>::from_decimal_str("4294967295");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 4294967295u32;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> =
// REWRITES-DAG:         bitint::BInt::<65, 2, 16>::from_decimal_str("18446744073709551615");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("1");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_mul({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 4294967295);
// REWRITES-DAG:     a = {{_v[0-9]+}}.0 as u32;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if ({{_v[0-9]+}} as i32) != ((18446744073709551615u64 > 4294967295u64) as i32) {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = a != 4294967295u32;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 8;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add(12 as i32);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -128 || {{_v[0-9]+}}.0 > 127);
// REWRITES-DAG:     b = {{_v[0-9]+}}.0 as i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = (b as i32) != 20;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("8");
// REWRITES-DAG:     let {{_v[0-9]+}}: bitint::BInt<65, 2, 16> = bitint::BInt::<65, 2, 16>::from_decimal_str("12");
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.to_i128().overflowing_sub({{_v[0-9]+}}.to_i128());
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -128 || {{_v[0-9]+}}.0 > 127);
// REWRITES-DAG:     b = {{_v[0-9]+}}.0 as i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = (b as i32) != -4;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul(3 as i32);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -128 || {{_v[0-9]+}}.0 > 127);
// REWRITES-DAG:     b = {{_v[0-9]+}}.0 as i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = (b as i32) != 6;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 8;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add(12 as i32);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 255);
// REWRITES-DAG:     c = {{_v[0-9]+}}.0 as u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = (c as i32) != 20;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: u64 = 8;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub(12 as u64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 255;
// REWRITES-DAG:     c = {{_v[0-9]+}}.0 as u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if ({{_v[0-9]+}} as i32) != ((18446744073709551612u64 > 252) as i32) {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = (c as i32) != 252;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul(3 as i32);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < 0 || ({{_v[0-9]+}}.0 as u128) > 255);
// REWRITES-DAG:     c = {{_v[0-9]+}}.0 as u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = (c as i32) != 6;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = 4294967295i64;
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = 4294967295i64;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// REWRITES-DAG:     d = {{_v[0-9]+}}.0 as i64;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if ({{_v[0-9]+}} as i32) != ((4294967295u64 + 1 < 4294967295u64) as i32) {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = d != ((2 * 4294967295u64) as i64);
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub(0 as i64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// REWRITES-DAG:     d = {{_v[0-9]+}}.0 as i64;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = d != 0;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i64 = 16;
// REWRITES-DAG:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul(1 as i64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// REWRITES-DAG:     d = {{_v[0-9]+}}.0 as i64;
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{_v[0-9]+}}: bool = d != 16;
// REWRITES-DAG:         {{_v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
