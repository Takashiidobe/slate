/* PR c/68513 */
/* { dg-do compile } */
/* { dg-options "-funsafe-math-optimizations -fno-math-errno -O -Wno-div-by-zero" } */

int i;
unsigned u;
volatile int *e;

#define E (i ? *e : 0)

/* Can't trigger some of them because operand_equal_p will return false
   for side-effects.  */

/* (x & ~m) | (y & m) -> ((x ^ y) & m) ^ x */
int
fn1 (void)
{
  int r = 0;
  r += (short) (E & ~u | i & u);
  r += -(short) (E & ~u | i & u);
  r += (short) -(E & ~u | i & u);
  return r;
}

/* sqrt(x) < y is x >= 0 && x != +Inf, when y is large.  */
double
fn2 (void)
{
  double r;
  r = __builtin_sqrt (E) < __builtin_inf ();
  return r;
}

/* sqrt(x) < c is the same as x >= 0 && x < c*c.  */
double
fn3 (void)
{
  double r;
  r = __builtin_sqrt (E) < 1.3;
  return r;
}

/* copysign(x,y)*copysign(x,y) -> x*x.  */
double
fn4 (double y, double x)
{
  return __builtin_copysign (E, y) * __builtin_copysign (E, y);
}

/* x <= +Inf is the same as x == x, i.e. !isnan(x).  */
int
fn5 (void)
{
  return E <= __builtin_inf ();
}

/* Fold (A & ~B) - (A & B) into (A ^ B) - B.  */
int
fn6 (void)
{
  return (i & ~E) - (i & E);
}

/* Fold (A & B) - (A & ~B) into B - (A ^ B).  */
int
fn7 (void)
{
  return (i & E) - (i & ~E);
}

/* x + (x & 1) -> (x + 1) & ~1 */
int
fn8 (void)
{
  return E + (E & 1);
}

/* Simplify comparison of something with itself.  */
int
fn9 (void)
{
  return E <= E | E >= E;
}

/* Fold (A & ~B) - (A & B) into (A ^ B) - B.  */
int
fn10 (void)
{
  return (i & ~E) - (i & E);
}

/* abs(x)*abs(x) -> x*x.  Should be valid for all types.  */
int
fn11 (void)
{
  return __builtin_abs (E) * __builtin_abs (E);
}

/* (x | CST1) & CST2 -> (x & CST2) | (CST1 & CST2) */
int
fn12 (void)
{
  return (E | 11) & 12;
}

/* fold_range_test */
int
fn13 (const char *s)
{
  return s[E] != '\0' && s[E] != '/';
}

/* fold_comparison */
int
fn14 (void)
{
  return (!!i ? : (u *= E / 0)) >= (u = E);
}

/* fold_mult_zconjz */
_Complex int
fn15 (_Complex volatile int *z)
{
  return *z * ~*z;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %0 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 u: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 e: ptr<volatile i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @fn1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(or<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), not<u32>(read<u32>(%1))), and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%0)), read<u32>(%1)))))));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%35));
// DEFAULT-NEXT:         let %36: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), neg<i32, overflow=ub>(widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(or<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), not<u32>(read<u32>(%1))), and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%0)), read<u32>(%1))))))));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%37));
// DEFAULT-NEXT:         let %38: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(or<u32>(and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), not<u32>(read<u32>(%1))), and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%0)), read<u32>(%1))))))));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%39));
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @__builtin_sqrt(%25 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %5 @fn2() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 r: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%6, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=assign>(lt<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%26, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), call<f64, signature=fn() -> f64>(%27)))));
// DEFAULT-NEXT:         int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=assign>(lt<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%26, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), call<f64, signature=fn() -> f64>(%27))));
// DEFAULT-NEXT:         return read<f64>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fn3() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 r: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%8, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=assign>(lt<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%26, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), const<f64>(1.3)))));
// DEFAULT-NEXT:         int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=assign>(lt<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%26, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), const<f64>(1.3))));
// DEFAULT-NEXT:         return read<f64>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @__builtin_copysign(%28 <unnamed>: f64, %29 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %9 @fn4(%10 y: f64, %11 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64, f64) -> f64>(%30, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), read<f64>(%10)), call<f64, signature=fn(f64, f64) -> f64>(%30, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), read<f64>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @fn5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f64, exceptions=observable>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), call<f64, signature=fn() -> f64>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @fn6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(and<i32>(read<i32>(%0), not<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), and<i32>(read<i32>(%0), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @fn7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(and<i32>(read<i32>(%0), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), and<i32>(read<i32>(%0), not<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @fn8() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)), and<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @fn9() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(from_bool<i32, reason=promotion>(le<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), from_bool<i32, reason=promotion>(ge<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @fn10() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(and<i32>(read<i32>(%0), not<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))), and<i32>(read<i32>(%0), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @__builtin_abs(%31 <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %18 @fn11() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%32, conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))), call<i32, signature=fn(i32) -> i32>(%32, conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @fn12() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(or<i32>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)), const<i32>(11)), const<i32>(12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @fn13(%21 s: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%21), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))))), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%21), conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))))), const<i32>(47))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @fn14() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33: bool [synthetic] = not<bool>(not<bool>(ne<i32>(read<i32>(%0), const<i32>(0))));
// DEFAULT-NEXT:         let %40: u32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<bool>(%33), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%40, reinterpret<u32, reason=usual_arith, fits=unknown>(read<bool>(%33)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %41: u32 [synthetic] = read<u32>(%1);
// DEFAULT-NEXT:             let %42: u32 [synthetic] = mul<u32, overflow=wrap>(read<u32>(%41), reinterpret<u32, reason=usual_arith, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)), const<i32>(0))));
// DEFAULT-NEXT:             write<u32>(%1, read<u32>(%42));
// DEFAULT-NEXT:             write<u32>(%40, read<u32>(%42));
// DEFAULT-NEXT:         write<u32>(%1, reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<u32>(read<u32>(%40), reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%2))), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @fn15(%24 z: ptr<volatile complex<i32>>) -> complex<i32> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<complex<i32>, complex=true, overflow=ub>(read<complex<i32>, volatile>(deref(read<ptr<volatile complex<i32>>>(%24))), not<complex<i32>, complex=true, overflow=ub>(read<complex<i32>, volatile>(deref(read<ptr<volatile complex<i32>>>(%24)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
