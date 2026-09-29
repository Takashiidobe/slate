int subject;
#ifdef C23_IS_KEYWORD
constexpr int constexpr_value = 5;
typeof_unqual(subject) typeof_unqual_derived;
#else
int constexpr = 5;
int typeof_unqual;
#endif
__typeof_unqual(subject) always_typeof_unqual_derived;

// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23 C23_IS_KEYWORD
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %[[VALUE_subject:[0-9]+]] subject: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_constexpr:[0-9]+]] constexpr: i32 [storage=static] = const<i32>(5) [linkage=external];
// C17-NEXT:     global %[[VALUE_typeof_unqual:[0-9]+]] typeof_unqual: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_always_typeof_unqual_derived:[0-9]+]] always_typeof_unqual_derived: i32 [storage=static] [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: module {
// GNU17-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-NEXT:         endian = little;
// GNU17-NEXT:         pointer [size=8, align=8];
// GNU17-NEXT:         stack_alignment = 16;
// GNU17-NEXT:         long_double = f80;
// GNU17-NEXT:         storage bool [size=1, align=1];
// GNU17-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-NEXT:         storage bf16 [size=2, align=2];
// GNU17-NEXT:         storage f16 [size=2, align=2];
// GNU17-NEXT:         storage f32 [size=4, align=4];
// GNU17-NEXT:         storage f64 [size=8, align=8];
// GNU17-NEXT:         storage f80 [size=16, align=16];
// GNU17-NEXT:         storage f128 [size=16, align=16];
// GNU17-NEXT:         storage d32 [size=4, align=4];
// GNU17-NEXT:         storage d64 [size=8, align=8];
// GNU17-NEXT:         storage d128 [size=16, align=16];
// GNU17-NEXT:     }
// GNU17-NEXT:     global %[[VALUE_subject:[0-9]+]] subject: i32 [storage=static] [linkage=external];
// GNU17-NEXT:     global %[[VALUE_constexpr:[0-9]+]] constexpr: i32 [storage=static] = const<i32>(5) [linkage=external];
// GNU17-NEXT:     global %[[VALUE_typeof_unqual:[0-9]+]] typeof_unqual: i32 [storage=static] [linkage=external];
// GNU17-NEXT:     global %[[VALUE_always_typeof_unqual_derived:[0-9]+]] always_typeof_unqual_derived: i32 [storage=static] [linkage=external];
// GNU17-NEXT: }
// SLATE-FILECHECK-END GNU17
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
// C23-NEXT:     global %[[VALUE_subject:[0-9]+]] subject: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_constexpr_value:[0-9]+]] constexpr_value: i32 [storage=static] [const] [constexpr] = const<i32>(5) [linkage=internal];
// C23-NEXT:     global %[[VALUE_typeof_unqual_derived:[0-9]+]] typeof_unqual_derived: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_always_typeof_unqual_derived:[0-9]+]] always_typeof_unqual_derived: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
