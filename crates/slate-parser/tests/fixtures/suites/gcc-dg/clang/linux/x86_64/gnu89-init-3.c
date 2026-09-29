/* PR 11527 */
/* { dg-do compile } */
/* { dg-options "-std=gnu89" } */

typedef struct smrdd_memory_blocks_s
{
  int blocks;
  int block[];
} smrdd_memory_blocks_t;

const smrdd_memory_blocks_t smrdd_memory_blocks =
{
  3,
  {
    [5] = 5,
    [1] = 2,
  }
};

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type[[TYPE_smrdd_memory_blocks_s:[0-9]+]] smrdd_memory_blocks_s = struct {
// DEFAULT-NEXT:         field0 blocks: i32;
// DEFAULT-NEXT:         field1 block: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_smrdd_memory_blocks_t:[0-9]+]] smrdd_memory_blocks_t = @type[[TYPE_smrdd_memory_blocks_s]];
// DEFAULT-NEXT:     global %[[VALUE_smrdd_memory_blocks:[0-9]+]] smrdd_memory_blocks: @type[[TYPE_smrdd_memory_blocks_s]] [storage=static] [const] = aggregate<@type[[TYPE_smrdd_memory_blocks_s]], zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<array<i32, 6>, zero_fill=true>(index1 = const<i32>(2), index5 = const<i32>(5))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
