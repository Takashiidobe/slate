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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: d128 [storage=static] = const<d128>(123456789135792468012345678900000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i128b [storage=static] = const<i128b>(123456789135792468012345678900000000000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: d64 [storage=static] = const<d64>(12345678913579000000000000000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i127b [storage=static] = widen<i127b, reason=assign>(const<i125b>(12345678913579000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: d128 [storage=static] = const<d128>(1234567891357924680123456789000000000000000000000000000000000000000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i256b [storage=static] = widen<i256b, reason=assign>(const<i251b>(1234567891357924680123456789000000000000000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: d64 [storage=static] = const<d64>(1234567891357900000000000000000000000000000000000000000000000000000000000000.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: i255b [storage=static] = widen<i255b, reason=assign>(const<i251b>(1234567891357900000000000000000000000000000000000000000000000000000000000000)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), int_to_float<d128, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i128b>(%[[VALUE_b]]))), ne<i128b>(float_to_int<i128b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_a]])), read<i128b>(%[[VALUE_b]]))), ne<d64, exceptions=observable>(read<d64>(%[[VALUE_c]]), int_to_float<d64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i127b>(%[[VALUE_d]])))), ne<i127b>(float_to_int<i127b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_c]])), read<i127b>(%[[VALUE_d]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: d128 [storage=automatic] = const<d128>(123456789135792468012345678900000000000.0);
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i128b [storage=automatic] = const<i128b>(123456789135792468012345678900000000000);
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: d128 [storage=automatic] = int_to_float<d128, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i128b>(123456789135792468012345678900000000000));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i128b [storage=automatic] = float_to_int<i128b, reason=assign, out_of_range=ub, exceptions=observable>(const<d128>(123456789135792468012345678900000000000.0));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: d64 [storage=automatic] = const<d64>(12345678913579000000000000000000000000.0);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i128b [storage=automatic] = widen<i128b, reason=assign>(const<i125b>(12345678913579000000000000000000000000));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i125b>(12345678913579000000000000000000000000));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i128b [storage=automatic] = float_to_int<i128b, reason=assign, out_of_range=ub, exceptions=observable>(const<d64>(12345678913579000000000000000000000000.0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%[[VALUE_e]]), read<d128>(%[[VALUE_g]])), ne<i128b>(read<i128b>(%[[VALUE_f]]), read<i128b>(%[[VALUE_h]]))), ne<d64, exceptions=observable>(read<d64>(%[[VALUE_i]]), read<d64>(%[[VALUE_k]]))), ne<i128b>(read<i128b>(%[[VALUE_j]]), read<i128b>(%[[VALUE_l]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%[[VALUE_m]]), int_to_float<d128, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i256b>(%[[VALUE_n]]))), ne<i256b>(float_to_int<i256b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_m]])), read<i256b>(%[[VALUE_n]]))), ne<d64, exceptions=observable>(read<d64>(%[[VALUE_o]]), int_to_float<d64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i255b>(%[[VALUE_p]])))), ne<i255b>(float_to_int<i255b, reason=explicit, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_o]])), read<i255b>(%[[VALUE_p]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: d128 [storage=automatic] = const<d128>(1234567891357924680123456789000000000000000000000000000000000000000000000000.0);
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i256b [storage=automatic] = widen<i256b, reason=assign>(const<i251b>(1234567891357924680123456789000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: d128 [storage=automatic] = int_to_float<d128, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i251b>(1234567891357924680123456789000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i256b [storage=automatic] = float_to_int<i256b, reason=assign, out_of_range=ub, exceptions=observable>(const<d128>(1234567891357924680123456789000000000000000000000000000000000000000000000000.0));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: d64 [storage=automatic] = const<d64>(1234567891357900000000000000000000000000000000000000000000000000000000000000.0);
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i255b [storage=automatic] = widen<i255b, reason=assign>(const<i251b>(1234567891357900000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: d64 [storage=automatic] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i251b>(1234567891357900000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i255b [storage=automatic] = float_to_int<i255b, reason=assign, out_of_range=ub, exceptions=observable>(const<d64>(1234567891357900000000000000000000000000000000000000000000000000000000000000.0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(read<d128>(%[[VALUE_q]]), read<d128>(%[[VALUE_s]])), ne<i256b>(read<i256b>(%[[VALUE_r]]), read<i256b>(%[[VALUE_t]]))), ne<d64, exceptions=observable>(read<d64>(%[[VALUE_u]]), read<d64>(%[[VALUE_w]]))), ne<i255b>(read<i255b>(%[[VALUE_v]]), read<i255b>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
