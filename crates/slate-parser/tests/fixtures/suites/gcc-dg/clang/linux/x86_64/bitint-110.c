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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i511b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: i31b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i31b [synthetic] = read<i31b>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i31b [synthetic] = truncate<i31b, reason=assign, fits=unknown>(rem<i511b, by_zero=ub, min_by_neg_one=ub>(widen<i511b, reason=usual_arith>(read<i31b>(%[[VALUE1]])), read<i511b>(%[[VALUE_b]])));
// DEFAULT-NEXT:                 write<i31b>(%[[VALUE_c]], read<i31b>(%[[VALUE2]]));
// DEFAULT-NEXT:                 label %[[VALUE_again:[0-9]+]] again:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i31b>(read<i31b>(%[[VALUE_c]]), const<i31b>(0));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i127b [synthetic] = read<i127b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i127b [synthetic] = div<i127b, by_zero=ub, min_by_neg_one=ub>(read<i127b>(%[[VALUE3]]), widen<i127b, reason=usual_arith>(const<i32>(0)));
// DEFAULT-NEXT:         write<i127b>(%[[VALUE_a]], read<i127b>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i31b [synthetic] = read<i31b>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i31b [synthetic] = truncate<i31b, reason=assign, fits=unknown>(sub<i127b, overflow=ub>(widen<i127b, reason=usual_arith>(read<i31b>(%[[VALUE5]])), read<i127b>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<i31b>(%[[VALUE_c]], read<i31b>(%[[VALUE6]]));
// DEFAULT-NEXT:         goto %[[VALUE_again]];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
