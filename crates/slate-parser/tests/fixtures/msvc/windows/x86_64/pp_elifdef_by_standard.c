// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

#define A
#undef B

#if 0
#elifdef A
int elifdef_applied;
#else
int elifdef_ignored;
#endif

#if 0
#elifndef B
int elifndef_applied;
#else
int elifndef_ignored;
#endif

#if 0
#if 1
#elifdef A
#endif
#endif

#if 1
int taken_before_elifdef;
#elifdef A
int elifdef_after_taken;
#endif

// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-pc-windows-msvc" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f64;
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
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %[[VALUE_elifdef_applied:[0-9]+]] elifdef_applied: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_elifndef_applied:[0-9]+]] elifndef_applied: i32 [storage=static] [linkage=external];
// C23-NEXT:     global %[[VALUE_taken_before_elifdef:[0-9]+]] taken_before_elifdef: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
