/* PR tree-optimization/114555 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O -fno-tree-forwprop" } */

#if __BITINT_MAXWIDTH__ >= 4139
struct S { _BitInt(31) : 6; _BitInt(513) b : 241; } s;
_BitInt(4139) a;
#endif

void
foo (void)
{
#if __BITINT_MAXWIDTH__ >= 4139
  int i = 0;
  a -= s.b << i;
#endif
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
// DEFAULT-NEXT:         field0 <anonymous>: i31b : 6;
// DEFAULT-NEXT:         field1 b: i513b : 241;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 0], bit_offsets=[Some(0), Some(6)], bit_units=[(0, 31)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i4139b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i4139b [synthetic] = read<i4139b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i4139b [synthetic] = sub<i4139b, overflow=ub>(read<i4139b>(%[[VALUE0]]), widen<i4139b, reason=usual_arith>(shl<i513b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i513b>(bitfield1<unit=0, bytes=0..31, bits=6..247>(%[[VALUE_s]])), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:         write<i4139b>(%[[VALUE_a]], read<i4139b>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
