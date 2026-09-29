int subject;
#ifdef TYPEOF_IS_KEYWORD
typeof(subject) typeof_derived;
#else
int typeof;
#endif
__typeof(subject) always_typeof_derived;

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES GNU89 TYPEOF_IS_KEYWORD
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17 TYPEOF_IS_KEYWORD
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23 TYPEOF_IS_KEYWORD
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %[[VALUE_subject:[0-9]+]] subject: i32 [storage=static] [linkage=external];
// C89-NEXT:     global %[[VALUE_typeof:[0-9]+]] typeof: i32 [storage=static] [linkage=external];
// C89-NEXT:     global %[[VALUE_always_typeof_derived:[0-9]+]] always_typeof_derived: i32 [storage=static] [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: module {
// GNU89-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU89-NEXT:         endian = little;
// GNU89-NEXT:         pointer [size=8, align=8];
// GNU89-NEXT:         stack_alignment = 16;
// GNU89-NEXT:         long_double = f80;
// GNU89-NEXT:         storage bool [size=1, align=1];
// GNU89-NEXT:         storage i8, u8 [size=1, align=1];
// GNU89-NEXT:         storage i16, u16 [size=2, align=2];
// GNU89-NEXT:         storage i32, u32 [size=4, align=4];
// GNU89-NEXT:         storage i64, u64 [size=8, align=8];
// GNU89-NEXT:         storage i128, u128 [size=16, align=16];
// GNU89-NEXT:         storage bf16 [size=2, align=2];
// GNU89-NEXT:         storage f16 [size=2, align=2];
// GNU89-NEXT:         storage f32 [size=4, align=4];
// GNU89-NEXT:         storage f64 [size=8, align=8];
// GNU89-NEXT:         storage f80 [size=16, align=16];
// GNU89-NEXT:         storage f128 [size=16, align=16];
// GNU89-NEXT:         storage d32 [size=4, align=4];
// GNU89-NEXT:         storage d64 [size=8, align=8];
// GNU89-NEXT:         storage d128 [size=16, align=16];
// GNU89-NEXT:     }
// GNU89-NEXT:     global %[[VALUE_subject:[0-9]+]] subject: i32 [storage=static] [linkage=external];
// GNU89-NEXT:     global %[[VALUE_typeof_derived:[0-9]+]] typeof_derived: i32 [storage=static] [linkage=external];
// GNU89-NEXT:     global %[[VALUE_always_typeof_derived:[0-9]+]] always_typeof_derived: i32 [storage=static] [linkage=external];
// GNU89-NEXT: }
// SLATE-FILECHECK-END GNU89
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
// C17-NEXT:     global %[[VALUE_typeof:[0-9]+]] typeof: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_always_typeof_derived:[0-9]+]] always_typeof_derived: i32 [storage=static] [linkage=external];
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
// GNU17-NEXT:     global %[[VALUE_typeof_derived:[0-9]+]] typeof_derived: i32 [storage=static] [linkage=external];
// GNU17-NEXT:     global %[[VALUE_always_typeof_derived:[0-9]+]] always_typeof_derived: i32 [storage=static] [linkage=external];
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
// C23-NEXT:     global %[[VALUE_typeof_derived:[0-9]+]] typeof_derived: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_always_typeof_derived:[0-9]+]] always_typeof_derived: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
