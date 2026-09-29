/* test that assignment drops qualifiers, Bug 98047 */
/* { dg-do compile } */
/* { dg-options "" } */


volatile int jv;
extern int j;
extern typeof(jv = 1) j;

_Atomic int ja;
extern typeof(ja = 1) j;

int * __restrict pa;
extern int *p;
extern typeof(pa = 0) p;




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
// DEFAULT-NEXT:     global %[[VALUE_jv:[0-9]+]] jv: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ja:[0-9]+]] ja: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pa:[0-9]+]] pa: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
