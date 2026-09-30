/* PR tree-optimization/113753 */
/* { dg-do compile { target bitint575 } } */
/* { dg-options "-std=gnu23" } */

_BitInt(161) a = 1461501637330902918203684832716283019655932542975wb / 2wb * 2wb;
_BitInt(160) b = 730750818665451459101842416358141509827966271487wb / 2wb * 2wb;
_BitInt(159) c = 365375409332725729550921208179070754913983135743wb / 2wb * 2wb;
_BitInt(129) d = 340282366920938463463374607431768211455wb / 2wb * 2wb;
_BitInt(128) e = 170141183460469231731687303715884105727wb / 2wb * 2wb;
_BitInt(161) f = (-1461501637330902918203684832716283019655932542975wb - 1wb) / 2wb * 2wb;
_BitInt(160) g = (-730750818665451459101842416358141509827966271487wb - 1wb) / 2wb * 2wb;
_BitInt(159) h = (-365375409332725729550921208179070754913983135743wb - 1wb) / 2wb * 2wb;
_BitInt(129) i = (-340282366920938463463374607431768211455wb - 1wb) / 2wb * 2wb;
_BitInt(128) j = (-170141183460469231731687303715884105727wb - 1wb) / 2wb * 2wb;
_BitInt(161) k = 1461501637330902918203684832716283019655932542975wb / 2wb * 3wb;		/* { dg-warning "integer overflow in expression of type '_BitInt\\\(161\\\)' results in" } */
_BitInt(160) l = 730750818665451459101842416358141509827966271487wb / 2wb * 3wb;		/* { dg-warning "integer overflow in expression of type '_BitInt\\\(160\\\)' results in" } */
_BitInt(159) m = 365375409332725729550921208179070754913983135743wb / 2wb * 3wb;		/* { dg-warning "integer overflow in expression of type '_BitInt\\\(159\\\)' results in" } */
_BitInt(129) n = 340282366920938463463374607431768211455wb / 2wb * 3wb;				/* { dg-warning "integer overflow in expression of type '_BitInt\\\(129\\\)' results in" } */
_BitInt(128) o = 170141183460469231731687303715884105727wb / 2wb * 3wb;				/* { dg-warning "integer overflow in expression of type '_BitInt\\\(128\\\)' results in" } */
_BitInt(161) p = (-1461501637330902918203684832716283019655932542975wb - 1wb) / 2wb * 3wb;	/* { dg-warning "integer overflow in expression of type '_BitInt\\\(161\\\)' results in" } */
_BitInt(160) q = (-730750818665451459101842416358141509827966271487wb - 1wb) / 2wb * 3wb;	/* { dg-warning "integer overflow in expression of type '_BitInt\\\(160\\\)' results in" } */
_BitInt(159) r = (-365375409332725729550921208179070754913983135743wb - 1wb) / 2wb * 3wb;	/* { dg-warning "integer overflow in expression of type '_BitInt\\\(159\\\)' results in" } */
_BitInt(129) s = (-340282366920938463463374607431768211455wb - 1wb) / 2wb * 3wb;		/* { dg-warning "integer overflow in expression of type '_BitInt\\\(129\\\)' results in" } */
_BitInt(128) t = (-170141183460469231731687303715884105727wb - 1wb) / 2wb * 3wb;		/* { dg-warning "integer overflow in expression of type '_BitInt\\\(128\\\)' results in" } */

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i161b [storage=static] = mul<i161b, overflow=ub>(div<i161b, by_zero=ub, min_by_neg_one=ub>(const<i161b>(1461501637330902918203684832716283019655932542975), widen<i161b, reason=usual_arith>(const<i3b>(2))), widen<i161b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i160b [storage=static] = mul<i160b, overflow=ub>(div<i160b, by_zero=ub, min_by_neg_one=ub>(const<i160b>(730750818665451459101842416358141509827966271487), widen<i160b, reason=usual_arith>(const<i3b>(2))), widen<i160b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i159b [storage=static] = mul<i159b, overflow=ub>(div<i159b, by_zero=ub, min_by_neg_one=ub>(const<i159b>(365375409332725729550921208179070754913983135743), widen<i159b, reason=usual_arith>(const<i3b>(2))), widen<i159b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i129b [storage=static] = mul<i129b, overflow=ub>(div<i129b, by_zero=ub, min_by_neg_one=ub>(const<i129b>(340282366920938463463374607431768211455), widen<i129b, reason=usual_arith>(const<i3b>(2))), widen<i129b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i128b [storage=static] = mul<i128b, overflow=ub>(div<i128b, by_zero=ub, min_by_neg_one=ub>(const<i128b>(170141183460469231731687303715884105727), widen<i128b, reason=usual_arith>(const<i3b>(2))), widen<i128b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i161b [storage=static] = mul<i161b, overflow=ub>(div<i161b, by_zero=ub, min_by_neg_one=ub>(sub<i161b, overflow=ub>(neg<i161b, overflow=ub>(const<i161b>(1461501637330902918203684832716283019655932542975)), widen<i161b, reason=usual_arith>(const<i2b>(1))), widen<i161b, reason=usual_arith>(const<i3b>(2))), widen<i161b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i160b [storage=static] = mul<i160b, overflow=ub>(div<i160b, by_zero=ub, min_by_neg_one=ub>(sub<i160b, overflow=ub>(neg<i160b, overflow=ub>(const<i160b>(730750818665451459101842416358141509827966271487)), widen<i160b, reason=usual_arith>(const<i2b>(1))), widen<i160b, reason=usual_arith>(const<i3b>(2))), widen<i160b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i159b [storage=static] = mul<i159b, overflow=ub>(div<i159b, by_zero=ub, min_by_neg_one=ub>(sub<i159b, overflow=ub>(neg<i159b, overflow=ub>(const<i159b>(365375409332725729550921208179070754913983135743)), widen<i159b, reason=usual_arith>(const<i2b>(1))), widen<i159b, reason=usual_arith>(const<i3b>(2))), widen<i159b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i129b [storage=static] = mul<i129b, overflow=ub>(div<i129b, by_zero=ub, min_by_neg_one=ub>(sub<i129b, overflow=ub>(neg<i129b, overflow=ub>(const<i129b>(340282366920938463463374607431768211455)), widen<i129b, reason=usual_arith>(const<i2b>(1))), widen<i129b, reason=usual_arith>(const<i3b>(2))), widen<i129b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i128b [storage=static] = mul<i128b, overflow=ub>(div<i128b, by_zero=ub, min_by_neg_one=ub>(sub<i128b, overflow=ub>(neg<i128b, overflow=ub>(const<i128b>(170141183460469231731687303715884105727)), widen<i128b, reason=usual_arith>(const<i2b>(1))), widen<i128b, reason=usual_arith>(const<i3b>(2))), widen<i128b, reason=usual_arith>(const<i3b>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i161b [storage=static] = mul<i161b, overflow=ub>(div<i161b, by_zero=ub, min_by_neg_one=ub>(const<i161b>(1461501637330902918203684832716283019655932542975), widen<i161b, reason=usual_arith>(const<i3b>(2))), widen<i161b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i160b [storage=static] = mul<i160b, overflow=ub>(div<i160b, by_zero=ub, min_by_neg_one=ub>(const<i160b>(730750818665451459101842416358141509827966271487), widen<i160b, reason=usual_arith>(const<i3b>(2))), widen<i160b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i159b [storage=static] = mul<i159b, overflow=ub>(div<i159b, by_zero=ub, min_by_neg_one=ub>(const<i159b>(365375409332725729550921208179070754913983135743), widen<i159b, reason=usual_arith>(const<i3b>(2))), widen<i159b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i129b [storage=static] = mul<i129b, overflow=ub>(div<i129b, by_zero=ub, min_by_neg_one=ub>(const<i129b>(340282366920938463463374607431768211455), widen<i129b, reason=usual_arith>(const<i3b>(2))), widen<i129b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i128b [storage=static] = mul<i128b, overflow=ub>(div<i128b, by_zero=ub, min_by_neg_one=ub>(const<i128b>(170141183460469231731687303715884105727), widen<i128b, reason=usual_arith>(const<i3b>(2))), widen<i128b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: i161b [storage=static] = mul<i161b, overflow=ub>(div<i161b, by_zero=ub, min_by_neg_one=ub>(sub<i161b, overflow=ub>(neg<i161b, overflow=ub>(const<i161b>(1461501637330902918203684832716283019655932542975)), widen<i161b, reason=usual_arith>(const<i2b>(1))), widen<i161b, reason=usual_arith>(const<i3b>(2))), widen<i161b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: i160b [storage=static] = mul<i160b, overflow=ub>(div<i160b, by_zero=ub, min_by_neg_one=ub>(sub<i160b, overflow=ub>(neg<i160b, overflow=ub>(const<i160b>(730750818665451459101842416358141509827966271487)), widen<i160b, reason=usual_arith>(const<i2b>(1))), widen<i160b, reason=usual_arith>(const<i3b>(2))), widen<i160b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: i159b [storage=static] = mul<i159b, overflow=ub>(div<i159b, by_zero=ub, min_by_neg_one=ub>(sub<i159b, overflow=ub>(neg<i159b, overflow=ub>(const<i159b>(365375409332725729550921208179070754913983135743)), widen<i159b, reason=usual_arith>(const<i2b>(1))), widen<i159b, reason=usual_arith>(const<i3b>(2))), widen<i159b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: i129b [storage=static] = mul<i129b, overflow=ub>(div<i129b, by_zero=ub, min_by_neg_one=ub>(sub<i129b, overflow=ub>(neg<i129b, overflow=ub>(const<i129b>(340282366920938463463374607431768211455)), widen<i129b, reason=usual_arith>(const<i2b>(1))), widen<i129b, reason=usual_arith>(const<i3b>(2))), widen<i129b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: i128b [storage=static] = mul<i128b, overflow=ub>(div<i128b, by_zero=ub, min_by_neg_one=ub>(sub<i128b, overflow=ub>(neg<i128b, overflow=ub>(const<i128b>(170141183460469231731687303715884105727)), widen<i128b, reason=usual_arith>(const<i2b>(1))), widen<i128b, reason=usual_arith>(const<i3b>(2))), widen<i128b, reason=usual_arith>(const<i3b>(3))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
