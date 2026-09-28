/* PR middle-end/120547 */
/* { dg-do run { target bitint } } */
/* { dg-options "-O2" } */
/* { dg-add-options float64 } */
/* { dg-require-effective-target float64 } */

#define CHECK(x, y) \
  if ((_Float64) x != (_Float64) y				\
      || (_Float64) (x + 1) != (_Float64) (y + 1))		\
    __builtin_abort ()

int
main ()
{
  unsigned long long a = 0x20000000000001ULL << 7;
  volatile unsigned long long b = a;
  CHECK (a, b);
#if __BITINT_MAXWIDTH__ >= 4096
  unsigned _BitInt(4096) c = ((unsigned _BitInt(4096)) 0x20000000000001ULL) << 253;
  volatile unsigned _BitInt(4096) d = c;
  CHECK (c, d);
  unsigned _BitInt(4096) e = ((unsigned _BitInt(4096)) 0x20000000000001ULL) << 931;
  volatile unsigned _BitInt(4096) f = e;
  CHECK (e, f);
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
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 a: u64 [storage=automatic] = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(9007199254740993), const<i32>(7));
// DEFAULT-NEXT:         let %2 b: volatile u64 [storage=automatic] = read<u64>(%1);
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u64>(%1)), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u64, volatile>(%2))), ne<f64, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(add<u64, overflow=wrap>(read<u64>(%1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(add<u64, overflow=wrap>(read<u64, volatile>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         let %3 c: u4096b [storage=automatic] = shl<u4096b, overflow=wrap, amount_out_of_range=ub>(widen<u4096b, reason=explicit>(const<u64>(9007199254740993)), const<i32>(253));
// DEFAULT-NEXT:         let %4 d: volatile u4096b [storage=automatic] = read<u4096b>(%3);
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u4096b>(%3)), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u4096b, volatile>(%4))), ne<f64, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(add<u4096b, overflow=wrap>(read<u4096b>(%3), reinterpret<u4096b, reason=usual_arith, fits=unknown>(widen<i4096b, reason=usual_arith>(const<i32>(1))))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(add<u4096b, overflow=wrap>(read<u4096b, volatile>(%4), reinterpret<u4096b, reason=usual_arith, fits=unknown>(widen<i4096b, reason=usual_arith>(const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         let %5 e: u4096b [storage=automatic] = shl<u4096b, overflow=wrap, amount_out_of_range=ub>(widen<u4096b, reason=explicit>(const<u64>(9007199254740993)), const<i32>(931));
// DEFAULT-NEXT:         let %6 f: volatile u4096b [storage=automatic] = read<u4096b>(%5);
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u4096b>(%5)), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(read<u4096b, volatile>(%6))), ne<f64, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(add<u4096b, overflow=wrap>(read<u4096b>(%5), reinterpret<u4096b, reason=usual_arith, fits=unknown>(widen<i4096b, reason=usual_arith>(const<i32>(1))))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(add<u4096b, overflow=wrap>(read<u4096b, volatile>(%6), reinterpret<u4096b, reason=usual_arith, fits=unknown>(widen<i4096b, reason=usual_arith>(const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
