#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
int sync_4 = 1;
#endif
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
int sync_8 = 1;
#endif
int gcc_llong = __GCC_ATOMIC_LLONG_LOCK_FREE;
int clang_llong = __CLANG_ATOMIC_LLONG_LOCK_FREE;

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES I386
// SLATE-FILECHECK-PREFIX-ARGS I386 -march=i386
// SLATE-FILECHECK-DEFINES I486
// SLATE-FILECHECK-PREFIX-ARGS I486 -march=i486
// SLATE-FILECHECK-DEFINES I586
// SLATE-FILECHECK-PREFIX-ARGS I586 -march=i586

// SLATE-FILECHECK-BEGIN I386
// I386: module {
// I386-NEXT:     target "i686-unknown-linux-gnu" {
// I386-NEXT:         endian = little;
// I386-NEXT:         pointer [size=4, align=4];
// I386-NEXT:         stack_alignment = 16;
// I386-NEXT:         long_double = f80;
// I386-NEXT:         storage bool [size=1, align=1];
// I386-NEXT:         storage i8, u8 [size=1, align=1];
// I386-NEXT:         storage i16, u16 [size=2, align=2];
// I386-NEXT:         storage i32, u32 [size=4, align=4];
// I386-NEXT:         storage i64, u64 [size=8, align=4];
// I386-NEXT:         storage i128, u128 [size=16, align=16];
// I386-NEXT:         storage bf16 [size=2, align=2];
// I386-NEXT:         storage f16 [size=2, align=2];
// I386-NEXT:         storage f32 [size=4, align=4];
// I386-NEXT:         storage f64 [size=8, align=4];
// I386-NEXT:         storage f80 [size=12, align=4];
// I386-NEXT:         storage f128 [size=16, align=16];
// I386-NEXT:         storage d32 [size=4, align=4];
// I386-NEXT:         storage d64 [size=8, align=8];
// I386-NEXT:         storage d128 [size=16, align=16];
// I386-NEXT:     }
// I386-NEXT:     global %[[VALUE_gcc_llong:[0-9]+]] gcc_llong: i32 [storage=static] = const<i32>(1) [linkage=external];
// I386-NEXT:     global %[[VALUE_clang_llong:[0-9]+]] clang_llong: i32 [storage=static] = const<i32>(1) [linkage=external];
// I386-NEXT: }
// SLATE-FILECHECK-END I386
// SLATE-FILECHECK-BEGIN I486
// I486: module {
// I486-NEXT:     target "i686-unknown-linux-gnu" {
// I486-NEXT:         endian = little;
// I486-NEXT:         pointer [size=4, align=4];
// I486-NEXT:         stack_alignment = 16;
// I486-NEXT:         long_double = f80;
// I486-NEXT:         storage bool [size=1, align=1];
// I486-NEXT:         storage i8, u8 [size=1, align=1];
// I486-NEXT:         storage i16, u16 [size=2, align=2];
// I486-NEXT:         storage i32, u32 [size=4, align=4];
// I486-NEXT:         storage i64, u64 [size=8, align=4];
// I486-NEXT:         storage i128, u128 [size=16, align=16];
// I486-NEXT:         storage bf16 [size=2, align=2];
// I486-NEXT:         storage f16 [size=2, align=2];
// I486-NEXT:         storage f32 [size=4, align=4];
// I486-NEXT:         storage f64 [size=8, align=4];
// I486-NEXT:         storage f80 [size=12, align=4];
// I486-NEXT:         storage f128 [size=16, align=16];
// I486-NEXT:         storage d32 [size=4, align=4];
// I486-NEXT:         storage d64 [size=8, align=8];
// I486-NEXT:         storage d128 [size=16, align=16];
// I486-NEXT:     }
// I486-NEXT:     global %[[VALUE_sync_4:[0-9]+]] sync_4: i32 [storage=static] = const<i32>(1) [linkage=external];
// I486-NEXT:     global %[[VALUE_gcc_llong:[0-9]+]] gcc_llong: i32 [storage=static] = const<i32>(1) [linkage=external];
// I486-NEXT:     global %[[VALUE_clang_llong:[0-9]+]] clang_llong: i32 [storage=static] = const<i32>(1) [linkage=external];
// I486-NEXT: }
// SLATE-FILECHECK-END I486
// SLATE-FILECHECK-BEGIN I586
// I586: module {
// I586-NEXT:     target "i686-unknown-linux-gnu" {
// I586-NEXT:         endian = little;
// I586-NEXT:         pointer [size=4, align=4];
// I586-NEXT:         stack_alignment = 16;
// I586-NEXT:         long_double = f80;
// I586-NEXT:         storage bool [size=1, align=1];
// I586-NEXT:         storage i8, u8 [size=1, align=1];
// I586-NEXT:         storage i16, u16 [size=2, align=2];
// I586-NEXT:         storage i32, u32 [size=4, align=4];
// I586-NEXT:         storage i64, u64 [size=8, align=4];
// I586-NEXT:         storage i128, u128 [size=16, align=16];
// I586-NEXT:         storage bf16 [size=2, align=2];
// I586-NEXT:         storage f16 [size=2, align=2];
// I586-NEXT:         storage f32 [size=4, align=4];
// I586-NEXT:         storage f64 [size=8, align=4];
// I586-NEXT:         storage f80 [size=12, align=4];
// I586-NEXT:         storage f128 [size=16, align=16];
// I586-NEXT:         storage d32 [size=4, align=4];
// I586-NEXT:         storage d64 [size=8, align=8];
// I586-NEXT:         storage d128 [size=16, align=16];
// I586-NEXT:     }
// I586-NEXT:     global %[[VALUE_sync_4:[0-9]+]] sync_4: i32 [storage=static] = const<i32>(1) [linkage=external];
// I586-NEXT:     global %[[VALUE_sync_8:[0-9]+]] sync_8: i32 [storage=static] = const<i32>(1) [linkage=external];
// I586-NEXT:     global %[[VALUE_gcc_llong:[0-9]+]] gcc_llong: i32 [storage=static] = const<i32>(2) [linkage=external];
// I586-NEXT:     global %[[VALUE_clang_llong:[0-9]+]] clang_llong: i32 [storage=static] = const<i32>(2) [linkage=external];
// I586-NEXT: }
// SLATE-FILECHECK-END I586
