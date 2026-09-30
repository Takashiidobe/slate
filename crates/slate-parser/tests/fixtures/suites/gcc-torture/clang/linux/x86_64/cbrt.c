/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

void abort(void);
void exit(int);

#ifndef __vax__
static const unsigned long B1 = 715094163, /* B1 = (682-0.03306235651)*2**20 */
    B2                        = 696219795; /* B2 = (664-0.03306235651)*2**20 */

static const double
    C = 5.42857142857142815906e-01,  /* 19/35     = 0x3FE15F15, 0xF15F15F1 */
    D = -7.05306122448979611050e-01, /* -864/1225 = 0xBFE691DE, 0x2532C834 */
    E = 1.41428571428571436819e+00,  /* 99/70     = 0x3FF6A0EA, 0x0EA0EA0F */
    F = 1.60714285714285720630e+00,  /* 45/28     = 0x3FF9B6DB, 0x6DB6DB6E */
    G = 3.57142857142857150787e-01;  /* 5/14      = 0x3FD6DB6D, 0xB6DB6DB7 */

double cbrtl(double x) {
  long             hx;
  double           r, s, w;
  double           lt;
  unsigned         sign;
  typedef unsigned unsigned32 __attribute__((mode(SI)));
  union {
    double     t;
    unsigned32 pt[2];
  } ut, ux;
  int n0;

  ut.t = 1.0;
  n0   = (ut.pt[0] == 0);

  ut.t = 0.0;
  ux.t = x;

  hx    = ux.pt[n0];       /* high word of x */
  sign  = hx & 0x80000000; /* sign= sign(x) */
  hx   ^= sign;
  if (hx >= 0x7ff00000)
    return (x + x); /* cbrt(NaN,INF) is itself */
  if ((hx | ux.pt[1 - n0]) == 0)
    return (ux.t); /* cbrt(0) is itself */

  ux.pt[n0] = hx;
  /* rough cbrt to 5 bits */
  if (hx < 0x00100000) /* subnormal number */
  {
    ut.pt[n0]  = 0x43500000; /* set t= 2**54 */
    ut.t      *= x;
    ut.pt[n0]  = ut.pt[n0] / 3 + B2;
  } else
    ut.pt[n0] = hx / 3 + B1;

  /* new cbrt to 23 bits, may be implemented in single precision */
  r     = ut.t * ut.t / ux.t;
  s     = C + r * ut.t;
  ut.t *= G + F / (s + E + D / s);

  /* chopped to 20 bits and make it larger than cbrt(x) */
  ut.pt[1 - n0]  = 0;
  ut.pt[n0]     += 0x00000001;

  /* one step newton iteration to 53 bits with error less than 0.667 ulps */
  s    = ut.t * ut.t; /* t*t is exact */
  r    = ux.t / s;
  w    = ut.t + ut.t;
  r    = (r - ut.t) / (w + r); /* r-s is exact */
  ut.t = ut.t + ut.t * r;

  /* restore the sign bit */
  ut.pt[n0] |= sign;

  lt  = ut.t;
  lt -= (lt - (x / (lt * lt))) * 0.333333333333333333333;
  return lt;
}

int main(void) {
  if ((int)(cbrtl(27.0) + 0.5) != 3)
    abort();

  exit(0);
}
#else
int main(void) { exit(0); }
#endif


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
// DEFAULT-NEXT:     type @type[[TYPE_unsigned32:[0-9]+]] unsigned32 = u32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 t: f64;
// DEFAULT-NEXT:         field1 pt: array<u32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_B1:[0-9]+]] B1: u64 [storage=static] [const] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(715094163))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_B2:[0-9]+]] B2: u64 [storage=static] [const] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(696219795))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_C:[0-9]+]] C: f64 [storage=static] [const] = const<f64>(0.5428571428571428) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_D:[0-9]+]] D: f64 [storage=static] [const] = neg<f64>(const<f64>(0.7053061224489796)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_E:[0-9]+]] E: f64 [storage=static] [const] = const<f64>(1.4142857142857144) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_F:[0-9]+]] F: f64 [storage=static] [const] = const<f64>(1.6071428571428572) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_G:[0-9]+]] G: f64 [storage=static] [const] = const<f64>(0.35714285714285715) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_cbrtl:[0-9]+]] @cbrtl(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_hx:[0-9]+]] hx: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lt:[0-9]+]] lt: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_sign:[0-9]+]] sign: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ut:[0-9]+]] ut: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ux:[0-9]+]] ux: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n0:[0-9]+]] n0: i32 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_ut]]), const<f64>(1.0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n0]], from_bool<i32, reason=assign>(eq<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_ut]]), const<f64>(0.0));
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_ux]]), read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_hx]], reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ux]])), read<i32>(%[[VALUE_n0]])))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_sign]], reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(and<i64>(read<i64>(%[[VALUE_hx]]), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(const<u32>(2147483648)))))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_hx]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = xor<i64>(read<i64>(%[[VALUE1]]), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_sign]]))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_hx]], read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:         if ge<i64>(read<i64>(%[[VALUE_hx]]), widen<i64, reason=usual_arith>(const<i32>(2146435072)))
// DEFAULT-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         if eq<i64>(or<i64>(read<i64>(%[[VALUE_hx]]), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ux]])), sub<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_n0]])))))))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return read<f64>(field0(%[[VALUE_ux]]));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ux]])), read<i32>(%[[VALUE_n0]]))), reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=unknown>(read<i64>(%[[VALUE_hx]]))));
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_hx]]), widen<i64, reason=usual_arith>(const<i32>(1048576)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), read<i32>(%[[VALUE_n0]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1129316352)));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: f64 [synthetic] = read<f64>(field0(%[[VALUE_ut]]));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: f64 [synthetic] = mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE3]]), read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:                 write<f64>(field0(%[[VALUE_ut]]), read<f64>(%[[VALUE4]]));
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), read<i32>(%[[VALUE_n0]]))), truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(div<u32, by_zero=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), read<i32>(%[[VALUE_n0]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))), read<u64>(%[[VALUE_B2]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), read<i32>(%[[VALUE_n0]]))), truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_hx]]), widen<i64, reason=usual_arith>(const<i32>(3)))), read<u64>(%[[VALUE_B1]]))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field0(%[[VALUE_ut]])), read<f64>(field0(%[[VALUE_ut]]))), read<f64>(field0(%[[VALUE_ux]]))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_s]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_C]]), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_r]]), read<f64>(field0(%[[VALUE_ut]])))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: f64 [synthetic] = read<f64>(field0(%[[VALUE_ut]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: f64 [synthetic] = mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE5]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_G]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_F]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_s]]), read<f64>(%[[VALUE_E]])), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_D]]), read<f64>(%[[VALUE_s]]))))));
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_ut]]), read<f64>(%[[VALUE6]]));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), sub<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_n0]])))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), read<i32>(%[[VALUE_n0]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE7]])));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE7]])), read<u32>(%[[VALUE9]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_s]], mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field0(%[[VALUE_ut]])), read<f64>(field0(%[[VALUE_ut]]))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field0(%[[VALUE_ux]])), read<f64>(%[[VALUE_s]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_w]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field0(%[[VALUE_ut]])), read<f64>(field0(%[[VALUE_ut]]))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_r]]), read<f64>(field0(%[[VALUE_ut]]))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_w]]), read<f64>(%[[VALUE_r]]))));
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_ut]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field0(%[[VALUE_ut]])), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field0(%[[VALUE_ut]])), read<f64>(%[[VALUE_r]]))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(2)>(field1(%[[VALUE_ut]])), read<i32>(%[[VALUE_n0]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE10]])));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = or<u32>(read<u32>(%[[VALUE11]]), read<u32>(%[[VALUE_sign]]));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE10]])), read<u32>(%[[VALUE12]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_lt]], read<f64>(field0(%[[VALUE_ut]])));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_lt]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE13]]), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_lt]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x]]), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_lt]]), read<f64>(%[[VALUE_lt]])))), const<f64>(0.3333333333333333)));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_lt]], read<f64>(%[[VALUE14]]));
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_lt]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_cbrtl]], const<f64>(27.0)), const<f64>(0.5))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
