#define VERSION 3
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PRESENT
#define ALIAS UNDEFINED_NAME
#if VERSION >= 2
int version_ok;
#endif
#if MAX(1, VERSION) == 3
int max_ok;
#endif
#if defined PRESENT && defined(ALIAS)
int defined_ok;
#endif
#ifdef WIDE
#define WIDTH 64
#else
#define WIDTH 32
#endif
#if WIDTH == 64
int wide;
#else
int narrow;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIDE WIDE

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
// DEFAULT-NEXT:     global %0 version_ok: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 max_ok: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 defined_ok: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 narrow: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIDE
// WIDE: module {
// WIDE-NEXT:     target "x86_64-unknown-linux-gnu" {
// WIDE-NEXT:         endian = little;
// WIDE-NEXT:         pointer [size=8, align=8];
// WIDE-NEXT:         stack_alignment = 16;
// WIDE-NEXT:         long_double = f80;
// WIDE-NEXT:         storage bool [size=1, align=1];
// WIDE-NEXT:         storage i8, u8 [size=1, align=1];
// WIDE-NEXT:         storage i16, u16 [size=2, align=2];
// WIDE-NEXT:         storage i32, u32 [size=4, align=4];
// WIDE-NEXT:         storage i64, u64 [size=8, align=8];
// WIDE-NEXT:         storage i128, u128 [size=16, align=16];
// WIDE-NEXT:         storage bf16 [size=2, align=2];
// WIDE-NEXT:         storage f16 [size=2, align=2];
// WIDE-NEXT:         storage f32 [size=4, align=4];
// WIDE-NEXT:         storage f64 [size=8, align=8];
// WIDE-NEXT:         storage f80 [size=16, align=16];
// WIDE-NEXT:         storage f128 [size=16, align=16];
// WIDE-NEXT:         storage d32 [size=4, align=4];
// WIDE-NEXT:         storage d64 [size=8, align=8];
// WIDE-NEXT:         storage d128 [size=16, align=16];
// WIDE-NEXT:     }
// WIDE-NEXT:     global %0 version_ok: i32 [storage=static] [linkage=external];
// WIDE-NEXT:     global %1 max_ok: i32 [storage=static] [linkage=external];
// WIDE-NEXT:     global %2 defined_ok: i32 [storage=static] [linkage=external];
// WIDE-NEXT:     global %3 wide: i32 [storage=static] [linkage=external];
// WIDE-NEXT: }
// SLATE-FILECHECK-END WIDE
