/* PR middle-end/116898 */
/* { dg-do compile { target bitint575 } } */
/* { dg-options "-O -finstrument-functions -fnon-call-exceptions" } */

_BitInt(127) a;
_BitInt(511) b;

void
foo (_BitInt(31) c)
{
  do
    {
      c %= b;
again:
    }
  while (c);
  a /= 0;		/* { dg-warning "division by zero" } */
  c -= a;
  goto again;
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
// DEFAULT-NEXT:     global %0 a: i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i511b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%4 c: i31b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %5
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %6: i31b [synthetic] = read<i31b>(%4);
// DEFAULT-NEXT:                 let %7: i31b [synthetic] = truncate<i31b, reason=assign, fits=unknown>(rem<i511b, by_zero=ub, min_by_neg_one=ub>(widen<i511b, reason=usual_arith>(read<i31b>(%6)), read<i511b>(%1)));
// DEFAULT-NEXT:                 write<i31b>(%4, read<i31b>(%7));
// DEFAULT-NEXT:                 label %3 again:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i31b>(read<i31b>(%4), const<i31b>(0));
// DEFAULT-NEXT:         let %8: i127b [synthetic] = read<i127b>(%0);
// DEFAULT-NEXT:         let %9: i127b [synthetic] = div<i127b, by_zero=ub, min_by_neg_one=ub>(read<i127b>(%8), widen<i127b, reason=usual_arith>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%0, read<i127b>(%9));
// DEFAULT-NEXT:         let %10: i31b [synthetic] = read<i31b>(%4);
// DEFAULT-NEXT:         let %11: i31b [synthetic] = truncate<i31b, reason=assign, fits=unknown>(sub<i127b, overflow=ub>(widen<i127b, reason=usual_arith>(read<i31b>(%10)), read<i127b>(%0)));
// DEFAULT-NEXT:         write<i31b>(%4, read<i31b>(%11));
// DEFAULT-NEXT:         goto %3;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
