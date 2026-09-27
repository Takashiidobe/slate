/* PR middle-end/120631 */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 128
_Decimal128 a = 123456789135792468012345678900000000000.0dl;
_BitInt(128) b = 123456789135792468012345678900000000000wb;
_Decimal64 c = 12345678913579000000000000000000000000.0dd;
_BitInt(127) d = 12345678913579000000000000000000000000wb;
#endif
#if __BITINT_MAXWIDTH__ >= 256
_Decimal128 m = 1234567891357924680123456789000000000000000000000000000000000000000000000000.0dl;
_BitInt(256) n = 1234567891357924680123456789000000000000000000000000000000000000000000000000wb;
_Decimal64 o = 1234567891357900000000000000000000000000000000000000000000000000000000000000.0dd;
_BitInt(255) p = 1234567891357900000000000000000000000000000000000000000000000000000000000000wb;
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 128
  if (a != b || (_BitInt(128)) a != b || c != d || (_BitInt(127)) c != d)
    __builtin_abort ();
  _Decimal128 e = 123456789135792468012345678900000000000.0dl;
  _BitInt(128) f = 123456789135792468012345678900000000000wb;
  _Decimal128 g = 123456789135792468012345678900000000000wb;
  _BitInt(128) h = 123456789135792468012345678900000000000.0dl;
  _Decimal64 i = 12345678913579000000000000000000000000.0dd;
  _BitInt(128) j = 12345678913579000000000000000000000000wb;
  _Decimal64 k = 12345678913579000000000000000000000000wb;
  _BitInt(128) l = 12345678913579000000000000000000000000.0dd;
  if (e != g || f != h || i != k || j != l)
    __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 256
  if (m != n || (_BitInt(256)) m != n || o != p || (_BitInt(255)) o != p)
    __builtin_abort ();
  _Decimal128 q = 1234567891357924680123456789000000000000000000000000000000000000000000000000.0dl;
  _BitInt(256) r = 1234567891357924680123456789000000000000000000000000000000000000000000000000wb;
  _Decimal128 s = 1234567891357924680123456789000000000000000000000000000000000000000000000000wb;
  _BitInt(256) t = 1234567891357924680123456789000000000000000000000000000000000000000000000000.0dl;
  _Decimal64 u = 1234567891357900000000000000000000000000000000000000000000000000000000000000.0dd;
  _BitInt(255) v = 1234567891357900000000000000000000000000000000000000000000000000000000000000wb;
  _Decimal64 w = 1234567891357900000000000000000000000000000000000000000000000000000000000000wb;
  _BitInt(255) x = 1234567891357900000000000000000000000000000000000000000000000000000000000000.0dd;
  if (q != s || r != t || u != w || v != x)
    __builtin_abort ();
#endif
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 a: d128 [storage=static] = const<d128>(123456789135792468012345678900000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i128b [storage=static] = const<i128b>(123456789135792468012345678900000000000) [linkage=external];
// DEFAULT-NEXT:     global %2 c: d64 [storage=static] = const<d64>(12345678913579000000000000000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i127b [storage=static] = widen<i127b, reason=assign>(const<i125b>(12345678913579000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %4 m: d128 [storage=static] = const<d128>(1234567891357924680123456789000000000000000000000000000000000000000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %5 n: i256b [storage=static] = widen<i256b, reason=assign>(const<i251b>(1234567891357924680123456789000000000000000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %6 o: d64 [storage=static] = const<d64>(1234567891357900000000000000000000000000000000000000000000000000000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %7 p: i255b [storage=static] = widen<i255b, reason=assign>(const<i251b>(1234567891357900000000000000000000000000000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%0), int_to_float<d128, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i128b>(%1))), ne<i128b>(float_to_int<i128b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128>(%0)), read<i128b>(%1))), ne<d64, exceptions=observable>(read<d64>(%2), int_to_float<d64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i127b>(%3)))), ne<i127b>(float_to_int<i127b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64>(%2)), read<i127b>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         let %9 e: d128 [storage=automatic] = const<d128>(123456789135792468012345678900000000000.0);
// DEFAULT-NEXT:         let %10 f: i128b [storage=automatic] = const<i128b>(123456789135792468012345678900000000000);
// DEFAULT-NEXT:         let %11 g: d128 [storage=automatic] = int_to_float<d128, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i128b>(123456789135792468012345678900000000000));
// DEFAULT-NEXT:         let %12 h: i128b [storage=automatic] = float_to_int<i128b, reason=assign, out_of_range=ub, exceptions=observable>(const<d128>(123456789135792468012345678900000000000.0));
// DEFAULT-NEXT:         let %13 i: d64 [storage=automatic] = const<d64>(12345678913579000000000000000000000000.0);
// DEFAULT-NEXT:         let %14 j: i128b [storage=automatic] = widen<i128b, reason=assign>(const<i125b>(12345678913579000000000000000000000000));
// DEFAULT-NEXT:         let %15 k: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i125b>(12345678913579000000000000000000000000));
// DEFAULT-NEXT:         let %16 l: i128b [storage=automatic] = float_to_int<i128b, reason=assign, out_of_range=ub, exceptions=observable>(const<d64>(12345678913579000000000000000000000000.0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%9), read<d128>(%11)), ne<i128b>(read<i128b>(%10), read<i128b>(%12))), ne<d64, exceptions=observable>(read<d64>(%13), read<d64>(%15))), ne<i128b>(read<i128b>(%14), read<i128b>(%16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%4), int_to_float<d128, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i256b>(%5))), ne<i256b>(float_to_int<i256b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128>(%4)), read<i256b>(%5))), ne<d64, exceptions=observable>(read<d64>(%6), int_to_float<d64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i255b>(%7)))), ne<i255b>(float_to_int<i255b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64>(%6)), read<i255b>(%7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:         let %17 q: d128 [storage=automatic] = const<d128>(1234567891357924680123456789000000000000000000000000000000000000000000000000.0);
// DEFAULT-NEXT:         let %18 r: i256b [storage=automatic] = widen<i256b, reason=assign>(const<i251b>(1234567891357924680123456789000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %19 s: d128 [storage=automatic] = int_to_float<d128, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i251b>(1234567891357924680123456789000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %20 t: i256b [storage=automatic] = float_to_int<i256b, reason=assign, out_of_range=ub, exceptions=observable>(const<d128>(1234567891357924680123456789000000000000000000000000000000000000000000000000.0));
// DEFAULT-NEXT:         let %21 u: d64 [storage=automatic] = const<d64>(1234567891357900000000000000000000000000000000000000000000000000000000000000.0);
// DEFAULT-NEXT:         let %22 v: i255b [storage=automatic] = widen<i255b, reason=assign>(const<i251b>(1234567891357900000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %23 w: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i251b>(1234567891357900000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %24 x: i255b [storage=automatic] = float_to_int<i255b, reason=assign, out_of_range=ub, exceptions=observable>(const<d64>(1234567891357900000000000000000000000000000000000000000000000000000000000000.0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%17), read<d128>(%19)), ne<i256b>(read<i256b>(%18), read<i256b>(%20))), ne<d64, exceptions=observable>(read<d64>(%21), read<d64>(%23))), ne<i255b>(read<i255b>(%22), read<i255b>(%24)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
