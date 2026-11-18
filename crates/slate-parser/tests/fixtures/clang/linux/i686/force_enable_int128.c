// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES FORCED
// SLATE-FILECHECK-PREFIX-ARGS FORCED -fforce-enable-int128
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef __SIZEOF_INT128__
int sizeof_int128 = __SIZEOF_INT128__;
#else
int sizeof_int128 = 0;
#endif

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=4];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=4];
// DEFAULT-NEXT:         storage f80 [size=12, align=4];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_sizeof_int128:[0-9]+]] sizeof_int128: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FORCED
// FORCED: module {
// FORCED-NEXT:     target "i686-unknown-linux-gnu" {
// FORCED-NEXT:         endian = little;
// FORCED-NEXT:         pointer [size=4, align=4];
// FORCED-NEXT:         stack_alignment = 16;
// FORCED-NEXT:         long_double = f80;
// FORCED-NEXT:         storage bool [size=1, align=1];
// FORCED-NEXT:         storage i8, u8 [size=1, align=1];
// FORCED-NEXT:         storage i16, u16 [size=2, align=2];
// FORCED-NEXT:         storage i32, u32 [size=4, align=4];
// FORCED-NEXT:         storage i64, u64 [size=8, align=4];
// FORCED-NEXT:         storage i128, u128 [size=16, align=16];
// FORCED-NEXT:         storage bf16 [size=2, align=2];
// FORCED-NEXT:         storage f16 [size=2, align=2];
// FORCED-NEXT:         storage f32 [size=4, align=4];
// FORCED-NEXT:         storage f64 [size=8, align=4];
// FORCED-NEXT:         storage f80 [size=12, align=4];
// FORCED-NEXT:         storage f128 [size=16, align=16];
// FORCED-NEXT:         storage d32 [size=4, align=4];
// FORCED-NEXT:         storage d64 [size=8, align=8];
// FORCED-NEXT:         storage d128 [size=16, align=16];
// FORCED-NEXT:     }
// FORCED-NEXT:     global %[[VALUE_sizeof_int128:[0-9]+]] sizeof_int128: i32 [storage=static] = const<i32>(16) [linkage=external];
// FORCED-NEXT: }
// SLATE-FILECHECK-END FORCED
