/* PR tree-optimization/114365 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

struct S {
  int : 31;
#if __BITINT_MAXWIDTH__ >= 129
  _BitInt(129) b : 129;
#else
  _BitInt(63) b : 63;
#endif
} s;

void
foo (int a)
{
  s.b <<= a;
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 <anonymous>: i32 : 31;
// DEFAULT-NEXT:         field1 b: i129b : 129;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 3], bit_offsets=[Some(0), Some(31)], bit_units=[(0, 20)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i129b [synthetic] = read<i129b>(bitfield1<unit=0, bytes=0..20, bits=31..160>(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i129b [synthetic] = shl<i129b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i129b>(%[[VALUE0]]), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i129b>(bitfield1<unit=0, bytes=0..20, bits=31..160>(%[[VALUE_s]]), read<i129b>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
