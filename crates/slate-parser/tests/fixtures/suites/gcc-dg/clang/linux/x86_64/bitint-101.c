/* PR tree-optimization/114329 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23" } */

#if __BITINT_MAXWIDTH__ >= 129
#define N 129
#else
#define N 63
#endif

struct S { _BitInt(N) b : N; } s;

void
foo (void)
{
  s.b ^= 42;
}

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
// DEFAULT-NEXT:         field0 b: i129b : 129;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 17)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %1 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3: i129b [synthetic] = read<i129b>(bitfield0<unit=0, bytes=0..17, bits=0..129>(%1));
// DEFAULT-NEXT:         let %4: i129b [synthetic] = xor<i129b>(read<i129b>(%3), widen<i129b, reason=usual_arith>(const<i32>(42)));
// DEFAULT-NEXT:         write<i129b>(bitfield0<unit=0, bytes=0..17, bits=0..129>(%1), read<i129b>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
