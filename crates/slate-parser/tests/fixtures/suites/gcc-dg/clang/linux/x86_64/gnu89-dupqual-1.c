/* { dg-do compile } */
/* { dg-options "-std=gnu89 -Werror" } */

typedef const int CI;
const const int c1;		/* { dg-bogus "duplicate" } */
const CI c2;			/* { dg-bogus "duplicate" } */
const CI *c3;			/* { dg-bogus "duplicate" } */

typedef volatile int VI;
volatile volatile int v1;	/* { dg-bogus "duplicate" } */
volatile VI v2;			/* { dg-bogus "duplicate" } */
volatile VI *v3;		/* { dg-bogus "duplicate" } */

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type[[TYPE_CI:[0-9]+]] CI = i32;
// DEFAULT-NEXT:     type @type[[TYPE_VI:[0-9]+]] VI = i32;
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c3:[0-9]+]] c3: ptr<const i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v3:[0-9]+]] v3: ptr<volatile i32> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
