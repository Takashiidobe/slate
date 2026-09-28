/* Test -fno-gnu89-extern-inline.  */
/* { dg-do compile } */
/* { dg-options "-std=c99 -fno-gnu89-inline" } */
/* { dg-final { scan-assembler-not "dontgenerate" } } */
/* { dg-final { scan-assembler "func1" } } */
/* { dg-final { scan-assembler "func2" } } */
/* { dg-final { scan-assembler "func3" } } */
/* { dg-final { scan-assembler "func4" } } */
/* { dg-final { scan-assembler "func5" } } */
/* { dg-final { scan-assembler "func6" } } */
/* { dg-final { scan-assembler "func7" } } */
/* { dg-final { scan-assembler "func8" } } */
/* { dg-final { scan-assembler "func9" } } */

#ifdef __GNUC_GNU_INLINE__
#error __GNUC_GNU_INLINE__ is defined
#endif

#ifndef __GNUC_STDC_INLINE__
#error __GNUC_STDC_INLINE__ is not defined
#endif

inline int dontgenerate1 (void)
{
  return 1;
}

inline int dontgenerate2 (void);
inline int dontgenerate2 (void)
{
  return 2;
}

inline int dontgenerate3 (void)
{
  return 3;
}
inline int dontgenerate3 (void);

extern inline int func1 (void) { return 1; }

extern inline int func2 (void);
inline int func2 (void) { return 2; }

inline int func3 (void) { return 3; }
extern inline int func3 (void);

inline int func4 (void);
extern inline int func4 (void) { return 4; }

extern inline int func5 (void) { return 5; }
inline int func5 (void);

extern int func6 (void);
inline int func6 (void) { return 6; }

inline int func7 (void) { return 7; }
extern int func7 (void);

inline int func8 (void);
extern int func8 (void) { return 8; }

extern int func9 (void) { return 9; }
inline int func9 (void);

// SLATE-FILECHECK-STD DEFAULT c99
// SLATE-FILECHECK-ARGS -fno-gnu89-inline
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
// DEFAULT-NEXT:     fn %0 @dontgenerate1() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @dontgenerate2() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @dontgenerate3() -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @func1() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @func2() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @func3() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @func4() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @func5() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @func6() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @func7() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @func8() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @func9() -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
