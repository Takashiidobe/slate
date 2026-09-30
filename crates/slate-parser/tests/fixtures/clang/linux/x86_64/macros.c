#define VALUE 7
#define TRANSITIVE VALUE
#define ID(x) x
#define JOIN(a, b) a ## b

int ordinary = TRANSITIVE;

#define SELF SELF
int recursive __attribute__((slate_literal(SELF)));

#ifdef SELECT
#define CHOICE 1
int selected __attribute__((slate_literal(CHOICE)));
#else
#define CHOICE 2
int selected __attribute__((slate_literal(CHOICE)));
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES SELECT SELECT

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
// DEFAULT-NEXT:     global %[[VALUE_ordinary:[0-9]+]] ordinary: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_recursive:[0-9]+]] recursive: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_selected:[0-9]+]] selected: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SELECT
// SELECT: module {
// SELECT-NEXT:     target "x86_64-unknown-linux-gnu" {
// SELECT-NEXT:         endian = little;
// SELECT-NEXT:         pointer [size=8, align=8];
// SELECT-NEXT:         stack_alignment = 16;
// SELECT-NEXT:         long_double = f80;
// SELECT-NEXT:         storage bool [size=1, align=1];
// SELECT-NEXT:         storage i8, u8 [size=1, align=1];
// SELECT-NEXT:         storage i16, u16 [size=2, align=2];
// SELECT-NEXT:         storage i32, u32 [size=4, align=4];
// SELECT-NEXT:         storage i64, u64 [size=8, align=8];
// SELECT-NEXT:         storage i128, u128 [size=16, align=16];
// SELECT-NEXT:         storage bf16 [size=2, align=2];
// SELECT-NEXT:         storage f16 [size=2, align=2];
// SELECT-NEXT:         storage f32 [size=4, align=4];
// SELECT-NEXT:         storage f64 [size=8, align=8];
// SELECT-NEXT:         storage f80 [size=16, align=16];
// SELECT-NEXT:         storage f128 [size=16, align=16];
// SELECT-NEXT:         storage d32 [size=4, align=4];
// SELECT-NEXT:         storage d64 [size=8, align=8];
// SELECT-NEXT:         storage d128 [size=16, align=16];
// SELECT-NEXT:     }
// SELECT-NEXT:     global %[[VALUE_ordinary:[0-9]+]] ordinary: i32 [storage=static] = const<i32>(7) [linkage=external];
// SELECT-NEXT:     global %[[VALUE_recursive:[0-9]+]] recursive: i32 [storage=static] [linkage=external];
// SELECT-NEXT:     global %[[VALUE_selected:[0-9]+]] selected: i32 [storage=static] [linkage=external];
// SELECT-NEXT: }
// SLATE-FILECHECK-END SELECT
