int B;

#ifdef AS_CAST
typedef int A;
int cast_main() {
  (A)(B);
  return 0;
}
#else
int A(int value);
int call_main() {
  (A)(B);
  return 0;
}
#endif
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES CAST AS_CAST

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
// DEFAULT-NEXT:     global %[[VALUE_B:[0-9]+]] B: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_A:[0-9]+]] @A(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call_main:[0-9]+]] @call_main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_A]], read<i32>(%[[VALUE_B]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN CAST
// CAST: module {
// CAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// CAST-NEXT:         endian = little;
// CAST-NEXT:         pointer [size=8, align=8];
// CAST-NEXT:         stack_alignment = 16;
// CAST-NEXT:         long_double = f80;
// CAST-NEXT:         storage bool [size=1, align=1];
// CAST-NEXT:         storage i8, u8 [size=1, align=1];
// CAST-NEXT:         storage i16, u16 [size=2, align=2];
// CAST-NEXT:         storage i32, u32 [size=4, align=4];
// CAST-NEXT:         storage i64, u64 [size=8, align=8];
// CAST-NEXT:         storage i128, u128 [size=16, align=16];
// CAST-NEXT:         storage bf16 [size=2, align=2];
// CAST-NEXT:         storage f16 [size=2, align=2];
// CAST-NEXT:         storage f32 [size=4, align=4];
// CAST-NEXT:         storage f64 [size=8, align=8];
// CAST-NEXT:         storage f80 [size=16, align=16];
// CAST-NEXT:         storage f128 [size=16, align=16];
// CAST-NEXT:         storage d32 [size=4, align=4];
// CAST-NEXT:         storage d64 [size=8, align=8];
// CAST-NEXT:         storage d128 [size=16, align=16];
// CAST-NEXT:     }
// CAST-NEXT:     type @type[[TYPE_A:[0-9]+]] A = i32;
// CAST-NEXT:     global %[[VALUE_B:[0-9]+]] B: i32 [storage=static] [linkage=external];
// CAST-NEXT:     fn %[[VALUE_cast_main:[0-9]+]] @cast_main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CAST-NEXT:         read<i32>(%[[VALUE_B]]);
// CAST-NEXT:         return const<i32>(0);
// CAST-NEXT:     }
// CAST-NEXT: }
// SLATE-FILECHECK-END CAST
