/* PR middle-end/112881 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23" } */

struct S { _BitInt(64) b; };

struct S
foo (_BitInt(64) p)
{
  return (struct S) { p };
}

#if __BITINT_MAXWIDTH__ >= 3924
struct T { _BitInt(3924) b; };

struct T
bar (_BitInt(3924) p)
{
  return (struct T) { p };
}
#endif

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 b: i64b;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 b: i3924b;
// DEFAULT-NEXT:     } [size=496, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo(%2 p: i64b) -> @type0 [linkage=external] [abi=sysv64(scalar) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(compound_literal %6 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i64b>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 p: i3924b) -> @type1 [linkage=external] [abi=sysv64(scalar) -> sret<align=8>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(compound_literal %7 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = read<i3924b>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
