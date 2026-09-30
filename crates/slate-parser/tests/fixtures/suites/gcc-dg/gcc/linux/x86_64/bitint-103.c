/* PR tree-optimization/114555 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -O2" } */

#if __BITINT_MAXWIDTH__ >= 1225
struct S { _BitInt(512) : 98; _BitInt(1225) b : 509; } s;
_BitInt(1225) a;
#endif

void
foo (void)
{
#if __BITINT_MAXWIDTH__ >= 1225
  a ^= (unsigned _BitInt(1025)) s.b;
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
// DEFAULT-NEXT:         field0 <anonymous>: i512b : 98;
// DEFAULT-NEXT:         field1 b: i1225b : 509;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 12], bit_offsets=[Some(0), Some(98)], bit_units=[(0, 76)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i1225b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i1225b [synthetic] = read<i1225b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i1225b [synthetic] = xor<i1225b>(read<i1225b>(%[[VALUE0]]), reinterpret<i1225b, reason=usual_arith, fits=unknown>(widen<u1225b, reason=usual_arith>(reinterpret<u1025b, reason=explicit, fits=unknown>(truncate<i1025b, reason=explicit, fits=unknown>(read<i1225b>(bitfield1<unit=0, bytes=0..76, bits=98..607>(%[[VALUE_s]])))))));
// DEFAULT-NEXT:         write<i1225b>(%[[VALUE_a]], read<i1225b>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
