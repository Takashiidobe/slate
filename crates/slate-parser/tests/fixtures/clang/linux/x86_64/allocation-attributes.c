int alloc(int size, int align) __attribute__((malloc, alloc_size(1 + 0, 2), alloc_align(2), returns_nonnull));
int result(void) __attribute__((warn_unused_result));
int variadic(int value, ...) __attribute__((sentinel));
__attribute__((assume_aligned(16, 4))) int aligned_result(void);

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
// DEFAULT-NEXT:     fn %[[VALUE_alloc:[0-9]+]] @alloc(%[[VALUE_size:[0-9]+]] size: i32, %[[VALUE_align:[0-9]+]] align: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_result:[0-9]+]] @result() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_value:[0-9]+]] value: i32, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_aligned_result:[0-9]+]] @aligned_result() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
