/* { dg-do run } */
/* { dg-options "-fno-inline -Os" } */
/* { dg-skip-if "long double support is incomplete" { "avr-*-*" } } */

void abort(void);


float powif(float x, int n)
{
  return __builtin_powif(x, n);
}

double powi(double x, int n)
{
  return __builtin_powi(x, n);
}

long double powil(long double x, int n)
{
  return __builtin_powil(x, n);
}


float powcif(float x)
{
  return __builtin_powif(x, 5);
}

double powci(double x)
{
  return __builtin_powi(x, 5);
}

long double powcil(long double x)
{
  return __builtin_powil(x, 5);
}


float powicf(int n)
{
  return __builtin_powif(2.0, n);
}

double powic(int n)
{
  return __builtin_powi(2.0, n);
}

long double powicl(int n)
{
  return __builtin_powil(2.0, n);
}


// @lowering-fn-begin
// @rewrite-fn-begin
int main()
{
  if (__builtin_powi(1.0, 5) != 1.0)
    abort();
  if (__builtin_powif(1.0, 5) != 1.0)
    abort();
  if (__builtin_powil(1.0, 5) != 1.0)
    abort();
  if (powci(1.0) != 1.0)
    abort();
  if (powcif(1.0) != 1.0)
    abort();
  if (powcil(1.0) != 1.0)
    abort();
  if (powi(1.0, -5) != 1.0)
    abort();
  if (powif(1.0, -5) != 1.0)
    abort();
  if (powil(1.0, -5) != 1.0)
    abort();
  if (powic(1) != 2.0)
    abort();
  if (powicf(1) != 2.0)
    abort();
  if (powicl(1) != 2.0)
    abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = unsafe { __slate_intrinsic_powi_25635816560c9105({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} as f32;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = unsafe { __slate_intrinsic_powi_cd10e01fde9d80b9({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = unsafe { __slate_f80_powi({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = powci({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} as f32;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = powcif({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = powcil({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = -5;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = powi({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} as f32;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = -5;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = powif({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = -5;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = powil({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = powic({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = powicf({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = powicl({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = unsafe { __slate_intrinsic_powi_25635816560c9105(1.0 as f64, 5 as i32) };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 1.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 =
// REWRITES-DAG:         unsafe { __slate_intrinsic_powi_cd10e01fde9d80b9((1.0 as f64) as f32, 5 as i32) };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as f64) != 1.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_f80_powi({{_v[0-9]+}}, 5 as i32) };
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = powci(1.0);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 1.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = powcif((1.0 as f64) as f32);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as f64) != 1.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = powcil({{_v[0-9]+}});
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = powi(1.0, -5);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 1.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = powif((1.0 as f64) as f32, -5);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as f64) != 1.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = powil({{_v[0-9]+}}, -5);
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = powic(1);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 2.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = powicf(1);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as f64) != 2.0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = powicl(1);
// REWRITES-DAG:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(2.0 as f64);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
