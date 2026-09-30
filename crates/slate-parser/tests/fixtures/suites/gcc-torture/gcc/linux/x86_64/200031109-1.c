// SLATE-FILECHECK-DEFINES DEFAULT

/* For a short time on the tree-ssa branch this would warn that
   value was not initialized as it was optimizing !(value = (m?1:2))
   to 0 and not setting value before.  */

int t(int m)
{
  int value;
  if (!(value = (m?1:2)))
    value = 0;
  return value;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_t:[0-9]+]] @t(%[[VALUE_m:[0-9]+]] m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = conditional<i32>(ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(0)), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE0]]), const<i32>(0)))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_value]], const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
