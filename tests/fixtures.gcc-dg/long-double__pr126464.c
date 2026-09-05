/* PR tree-optimization/126464 */
/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-add-options ieee } */
/* { dg-skip-if "not IEEE float" { "pdp11-*-*" } } */

[[gnu::noipa]] double foo(double x) {
  float y = (float)x;

  if (y == -__builtin_inff())
    return x * 0.5;
  return y;
}

[[gnu::noipa]] long double bar(long double x) {
  double y = (double)x;

  if (y == __builtin_inf())
    return x * 0.5L;
  return y;
}

[[gnu::noipa]] double baz(double x) {
  float y = (float)x;

  if (y == __builtin_inff())
    return x * 0.5;
  return y;
}

[[gnu::noipa]] long double qux(long double x) {
  double y = (double)x;

  if (y == -__builtin_inf())
    return x * 0.5L;
  return y;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
#if __DBL_MAX_10_EXP__ >= 301
  if (!__builtin_isinf((double)1e300) && __builtin_isinf((float)1e300) &&
      (foo(-1e300) != -5e299 || baz(1e300) != 5e299))
    __builtin_abort();
#endif

#if __LDBL_MAX_10_EXP__ >= 4001
  if (!__builtin_isinf((long double)1e4000L) &&
      __builtin_isinf((double)1e4000L) &&
      (bar(1e4000L) != 5e3999L || qux(-1e4000L) != -5e3999L))
    __builtin_abort();
#endif
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = -1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = foo({{__v[0-9]+}});
// LOWERING-DAG:             let {{__v[0-9]+}}: f64 = -500000000000000026252380127602210124352234290554079577457927057755901228994454097893185687540223932021852221916441939088471261617680215287822396092393353491424193600463287901868915116897394045029684476617485399972540559519483820440037326371390071247289629394410028421419057834736098193432729700270080.0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                 {{__v[0-9]+}}
// LOWERING-DAG:             } else {
// LOWERING-DAG:                 let {{__v[0-9]+}}: f64 = 1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: f64 = baz({{__v[0-9]+}});
// LOWERING-DAG:                 let {{__v[0-9]+}}: f64 = 500000000000000026252380127602210124352234290554079577457927057755901228994454097893185687540223932021852221916441939088471261617680215287822396092393353491424193600463287901868915116897394045029684476617485399972540559519483820440037326371390071247289629394410028421419057834736098193432729700270080.0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 {{__v[0-9]+}}
// LOWERING-DAG:             };
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 230, 115]);
// LOWERING-DAG:             let {{__v[0-9]+}}: LongDouble = bar({{__v[0-9]+}});
// LOWERING-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 229, 115]);
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                 {{__v[0-9]+}}
// LOWERING-DAG:             } else {
// LOWERING-DAG:                 let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 230, 243]);
// LOWERING-DAG:                 let {{__v[0-9]+}}: LongDouble = qux({{__v[0-9]+}});
// LOWERING-DAG:                 let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 229, 243]);
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 {{__v[0-9]+}}
// LOWERING-DAG:             };
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = -1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = foo({{__v[0-9]+}});
// REWRITES-DAG:         let {{__v[0-9]+}}: f64 = -500000000000000026252380127602210124352234290554079577457927057755901228994454097893185687540223932021852221916441939088471261617680215287822396092393353491424193600463287901868915116897394045029684476617485399972540559519483820440037326371390071247289629394410028421419057834736098193432729700270080.0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: f64 = 1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// REWRITES-DAG:             let {{__v[0-9]+}}: f64 = baz({{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: f64 = 500000000000000026252380127602210124352234290554079577457927057755901228994454097893185687540223932021852221916441939088471261617680215287822396092393353491424193600463287901868915116897394045029684476617485399972540559519483820440037326371390071247289629394410028421419057834736098193432729700270080.0;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 230, 115]);
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = bar({{__v[0-9]+}});
// REWRITES-DAG:         let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 229, 115]);
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 230, 243]);
// REWRITES-DAG:             let {{__v[0-9]+}}: LongDouble = qux({{__v[0-9]+}});
// REWRITES-DAG:             let {{__v[0-9]+}}: LongDouble = LongDouble([97, 140, 85, 254, 35, 131, 186, 209, 229, 243]);
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
