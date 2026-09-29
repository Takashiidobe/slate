/* Test C23 typeof and typeof_unqual on arrays of atomic elements (bug
   117781).  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Atomic int a[2], b[2][2];
const _Atomic int c[2], d[2][2];

extern typeof (a) a;
extern typeof (b) b;
extern typeof (c) c;
extern typeof (d) d;
extern typeof_unqual (a) a;
extern typeof_unqual (b) b;
extern typeof_unqual (c) a;
extern typeof_unqual (d) b;
extern typeof_unqual (volatile _Atomic int [2]) a;
extern typeof_unqual (volatile _Atomic int [2][2]) b;

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: atomic array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: atomic array<array<i32, 2>, 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: atomic array<i32, 2> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: atomic array<array<i32, 2>, 2> [storage=static] [const] [align=16] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
