// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

int empty();
int explicit_void(void);

int calls(void) {
    return empty() + explicit_void();
}

// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_empty:[0-9]+]] @empty() -> i32 [linkage=external] [c="int(void)"];
// C23-NEXT:     fn %[[VALUE_explicit_void:[0-9]+]] @explicit_void() -> i32 [linkage=external] [c="int(void)"];
// C23-NEXT:     fn %[[VALUE_calls:[0-9]+]] @calls() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// C23-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_empty]]), call<i32, signature=fn() -> i32>(%[[VALUE_explicit_void]]));
// C23-NEXT:     }
// C23-NEXT: }
// SLATE-FILECHECK-END C23
