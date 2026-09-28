#ifdef SELECT
#define VALUE 1
#else
#define VALUE 2
#endif

#ifdef SELECT
#define PICK(value) value
#else
#define PICK(value) 4
#endif

#ifdef SELECT
#define TYPE int
#else
#define TYPE char
#endif

int selected = VALUE;
TYPE typed;

int picked(void) {
    return PICK(3);
}

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
// DEFAULT-NEXT:     global %0 selected: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %1 typed: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @picked() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
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
// SELECT-NEXT:     global %0 selected: i32 [storage=static] = const<i32>(1) [linkage=external];
// SELECT-NEXT:     global %1 typed: i32 [storage=static] [linkage=external];
// SELECT-NEXT:     fn %2 @picked() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SELECT-NEXT:         return const<i32>(3);
// SELECT-NEXT:     }
// SELECT-NEXT: }
// SLATE-FILECHECK-END SELECT
