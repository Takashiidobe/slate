/* { dg-do compile { target fpic } } */
/* { dg-options "-O2 -fPIC" } */

volatile _Decimal32 d;
volatile int i;

void foo()
{
  d += i;
  d += i;
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
// DEFAULT-NEXT:     global %0 d: volatile d32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 i: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3: d32 [synthetic] = read<d32, volatile>(%0);
// DEFAULT-NEXT:         let %4: d32 [synthetic] = add<d32, rounding=nearest_even, exceptions=observable, contract=fast>(read<d32>(%3), int_to_float<d32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i32, volatile>(%1)));
// DEFAULT-NEXT:         write<d32, volatile>(%0, read<d32>(%4));
// DEFAULT-NEXT:         let %5: d32 [synthetic] = read<d32, volatile>(%0);
// DEFAULT-NEXT:         let %6: d32 [synthetic] = add<d32, rounding=nearest_even, exceptions=observable, contract=fast>(read<d32>(%5), int_to_float<d32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i32, volatile>(%1)));
// DEFAULT-NEXT:         write<d32, volatile>(%0, read<d32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
