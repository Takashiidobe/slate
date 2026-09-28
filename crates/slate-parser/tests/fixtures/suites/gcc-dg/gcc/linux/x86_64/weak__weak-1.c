/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-fno-common" } */
/* { dg-skip-if "" { *-*-mingw* } } */
/* NVPTX's definition of weak looks different to normal.  */
/* { dg-skip-if "" { nvptx-*-* } } */
/* { dg-skip-if PR119369 { amdgcn-*-* } } */

/* { dg-final { scan-weak "a" } } */
/* { dg-final { scan-weak "b" } } */
/* { dg-final { scan-weak "c" } } */
/* { dg-final { scan-weak "d" } } */
/* { dg-final { scan-weak "e" } } */
/* { dg-final { scan-weak "g" } } */
/* { dg-final { scan-not-weak "i" } } */
/* { dg-final { scan-weak "j" } } */

#pragma weak a
int a;

int b;
#pragma weak b

#pragma weak c
extern int c;
int c;

extern int d;
#pragma weak d
int d;

#pragma weak e
void e(void) { }

#if 0
/* This permutation is illegal.  */
void f(void) { }
#pragma weak f
#endif

#pragma weak g
int g = 1;

#if 0
/* This permutation is illegal.  */
int h = 1;
#pragma weak h
#endif

#pragma weak i
extern int i;

#pragma weak j
extern int j;
int use_j() { return j; }

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-common
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %5 g: i32 [storage=static] = const<i32>(1) [linkage=external] [weak];
// DEFAULT-NEXT:     extern %6 i: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %7 j: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     fn %4 @e() -> void [linkage=external] [weak] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @use_j() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
