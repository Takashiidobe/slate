#define TRY(n) ({ __label__ failed; int e = 1; asm goto("" :::: failed); e = 0; failed: e + (n); })

int f(int n) {
  if (TRY(n))
    return 1;
  return ({ int r = TRY(n); r; });
}



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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             asm goto "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:                 labels: %[[VALUE_failed:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<i32>(%[[VALUE_e]], const<i32>(0));
// DEFAULT-NEXT:             label %[[VALUE_failed]] failed:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], add<i32, overflow=ub>(read<i32>(%[[VALUE_e]]), read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE0]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_e_2:[0-9]+]] e: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:                 asm goto "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:                     labels: %[[VALUE_failed_2:[0-9]+]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_2]], const<i32>(0));
// DEFAULT-NEXT:                 label %[[VALUE_failed_2]] failed:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                 write<i32>(%[[VALUE2]], add<i32, overflow=ub>(read<i32>(%[[VALUE_e_2]]), read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], read<i32>(%[[VALUE_r]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
