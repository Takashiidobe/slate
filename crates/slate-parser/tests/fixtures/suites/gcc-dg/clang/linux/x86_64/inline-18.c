/* Test -fgnu89-extern-inline.  */
/* { dg-do compile } */
/* { dg-options "-std=c99 -fgnu89-inline" } */
/* { dg-final { scan-assembler "func1" } } */
/* { dg-final { scan-assembler-not "func2" } } */
/* { dg-final { scan-assembler "func3" } } */
/* { dg-final { scan-assembler "func4" } } */

#ifndef __GNUC_GNU_INLINE__
#error __GNUC_GNU_INLINE__ is not defined
#endif

#ifdef __GNUC_STDC_INLINE__
#error __GNUC_STDC_INLINE__ is defined
#endif

extern inline int func1 (void) { return 0; }
inline int func1 (void) { return 1; }

extern int func2 (void);
extern inline int func2 (void) { return 2; }

inline int func3 (void);
inline int func3 (void) { return 3; }

extern int func4 (void);
extern inline int func4 (void) { return 4; }
int func4 (void) { return 5; }

// SLATE-FILECHECK-STD DEFAULT c99
// SLATE-FILECHECK-ARGS -fgnu89-inline
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
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func3:[0-9]+]] @func3() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func4:[0-9]+]] @func4() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
