/* PR tree-optimization/116093 */
/* { dg-do run { target bitint } } */
/* { dg-options "-Og -ftree-vrp -fno-tree-dce" } */

#if __BITINT_MAXWIDTH__ >= 129
char
foo (int a, _BitInt (129) b, char c)
{
  return c << (5 / b % (0xdb75dbf5 | a));
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 129
  if (foo (0, 6, 1) != 1)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i32, %2 b: i129b, %3 c: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%3)), rem<i129b, by_zero=ub, min_by_neg_one=ub>(div<i129b, by_zero=ub, min_by_neg_one=ub>(widen<i129b, reason=usual_arith>(const<i32>(5)), read<i129b>(%2)), reinterpret<i129b, reason=usual_arith, fits=unknown>(widen<u129b, reason=usual_arith>(or<u32>(const<u32>(3681934325), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i32, i129b, i8) -> i8>(%0, const<i32>(0), widen<i129b, reason=arg>(const<i32>(6)), truncate<i8, reason=arg, fits=always>(const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
