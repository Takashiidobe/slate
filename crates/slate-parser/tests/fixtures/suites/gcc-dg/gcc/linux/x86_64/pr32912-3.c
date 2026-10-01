/* { dg-do compile } */
/* { dg-options "-w -O2 -fdump-tree-optimized" } */

typedef int __m128i __attribute__ ((__vector_size__ (16)));

__m128i
bar (void)
{
  __m128i x = { 0x11111111, 0x22222222, 0x44444444 };
  return ~x;
}

/* { dg-final { scan-tree-dump-not "~\{" "optimized" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE___m128i:[0-9]+]] __m128i = vector<i32, 4>;
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> vector<i32, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=true>(index0 = const<i32>(286331153), index1 = const<i32>(572662306), index2 = const<i32>(1145324612));
// DEFAULT-NEXT:         return not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
