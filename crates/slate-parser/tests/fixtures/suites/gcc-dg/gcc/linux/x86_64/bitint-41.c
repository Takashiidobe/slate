/* PR middle-end/112336 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23" } */

unsigned _BitInt(1) v1;
unsigned _BitInt(1) *p1 = &v1;
signed _BitInt(2) v2;
signed _BitInt(2) *p2 = &v2;
unsigned _BitInt(11) v11;
unsigned _BitInt(11) *p11 = &v11;
signed _BitInt(12) v12;
signed _BitInt(12) *p12 = &v12;
unsigned _BitInt(21) v21;
unsigned _BitInt(21) *p21 = &v21;
signed _BitInt(22) v22;
signed _BitInt(22) *p22 = &v22;
unsigned _BitInt(31) v31;
unsigned _BitInt(31) *p31 = &v31;
signed _BitInt(32) v32;
signed _BitInt(32) *p32 = &v32;
unsigned _BitInt(41) v41;
unsigned _BitInt(41) *p41 = &v41;
signed _BitInt(42) v42;
signed _BitInt(42) *p42 = &v42;
#if __BITINT_MAXWIDTH__ >= 128
unsigned _BitInt(127) v127;
unsigned _BitInt(127) *p127 = &v127;
signed _BitInt(128) v128;
signed _BitInt(128) *p128 = &v128;
#endif
#if __BITINT_MAXWIDTH__ >= 258
unsigned _BitInt(257) v257;
unsigned _BitInt(257) *p257 = &v257;
signed _BitInt(258) v258;
signed _BitInt(258) *p258 = &v258;
#endif

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
// DEFAULT-NEXT:     global %0 v1: u1b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 p1: ptr<u1b> [storage=static] = addr_of<ptr<u1b>>(%0) [linkage=external];
// DEFAULT-NEXT:     global %2 v2: i2b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 p2: ptr<i2b> [storage=static] = addr_of<ptr<i2b>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %4 v11: u11b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 p11: ptr<u11b> [storage=static] = addr_of<ptr<u11b>>(%4) [linkage=external];
// DEFAULT-NEXT:     global %6 v12: i12b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 p12: ptr<i12b> [storage=static] = addr_of<ptr<i12b>>(%6) [linkage=external];
// DEFAULT-NEXT:     global %8 v21: u21b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 p21: ptr<u21b> [storage=static] = addr_of<ptr<u21b>>(%8) [linkage=external];
// DEFAULT-NEXT:     global %10 v22: i22b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 p22: ptr<i22b> [storage=static] = addr_of<ptr<i22b>>(%10) [linkage=external];
// DEFAULT-NEXT:     global %12 v31: u31b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 p31: ptr<u31b> [storage=static] = addr_of<ptr<u31b>>(%12) [linkage=external];
// DEFAULT-NEXT:     global %14 v32: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 p32: ptr<i32b> [storage=static] = addr_of<ptr<i32b>>(%14) [linkage=external];
// DEFAULT-NEXT:     global %16 v41: u41b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 p41: ptr<u41b> [storage=static] = addr_of<ptr<u41b>>(%16) [linkage=external];
// DEFAULT-NEXT:     global %18 v42: i42b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 p42: ptr<i42b> [storage=static] = addr_of<ptr<i42b>>(%18) [linkage=external];
// DEFAULT-NEXT:     global %20 v127: u127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %21 p127: ptr<u127b> [storage=static] = addr_of<ptr<u127b>>(%20) [linkage=external];
// DEFAULT-NEXT:     global %22 v128: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 p128: ptr<i128b> [storage=static] = addr_of<ptr<i128b>>(%22) [linkage=external];
// DEFAULT-NEXT:     global %24 v257: u257b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 p257: ptr<u257b> [storage=static] = addr_of<ptr<u257b>>(%24) [linkage=external];
// DEFAULT-NEXT:     global %26 v258: i258b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 p258: ptr<i258b> [storage=static] = addr_of<ptr<i258b>>(%26) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
