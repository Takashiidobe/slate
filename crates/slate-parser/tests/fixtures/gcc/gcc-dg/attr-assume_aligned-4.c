/* PR middle-end/87533 - bogus assume_aligned attribute silently accepted
   { dg-do compile }
   { dg-options "-Wall" } */

#define A(...)  __attribute__ ((assume_aligned (__VA_ARGS__)))

A (1) void fv_1 (void);       /* { dg-warning ".assume_aligned. attribute ignored on a function returning .void." } */

A (1) int fi_1 (void);        /* { dg-warning ".assume_aligned. attribute ignored on a function returning .int." } */

A (-1) void* fpv_m1 (void);   /* { dg-warning ".assume_aligned. attribute argument -1 is not positive" } */

A (0) void* fpv_0 (void);     /* { dg-warning ".assume_aligned. attribute argument 0 is not a power of 2" } */

/* Alignment of 1 is fine, it just doesn't offer any benefits.  */
A (1) void* fpv_1 (void);

A (3) void* fpv_3 (void);     /* { dg-warning ".assume_aligned. attribute argument 3 is not a power of 2" } */

A (16383) void* fpv_16km1 (void);     /* { dg-warning ".assume_aligned. attribute argument 16383 is not a power of 2" } */
A (16384) void* fpv_16k (void);
A (16385) void* fpv_16kp1 (void);    /* { dg-warning ".assume_aligned. attribute argument 16385 is not a power of 2" } */

A (32767) void* fpv_32km1 (void);     /* { dg-warning ".assume_aligned. attribute argument 32767 is not a power of 2" } */

A (4, -1) void* fpv_4_m1 (void);      /* { dg-warning ".assume_aligned. attribute argument -1 is not positive" } */

A (4, 0) void* fpv_4_0 (void);
A (4, 1) void* fpv_4_1 (void);
A (4, 2) void* fpv_4_2 (void);
A (4, 3) void* fpv_4_3 (void);

A (4, 4) void* fpv_4_3 (void);        /* { dg-warning ".assume_aligned. attribute argument 4 is not in the range \\\[0, 3]" } */

A (4) void* gpv_4_3 (void);
A (2) void* gpv_4_3 (void);

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %0 @fv_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @fi_1() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @fpv_m1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @fpv_0() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @fpv_1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @fpv_3() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @fpv_16km1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @fpv_16k() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @fpv_16kp1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @fpv_32km1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @fpv_4_m1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %11 @fpv_4_0() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @fpv_4_1() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @fpv_4_2() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @fpv_4_3() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @gpv_4_3() -> ptr<void> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
