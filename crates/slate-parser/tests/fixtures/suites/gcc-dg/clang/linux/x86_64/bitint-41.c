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
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: u1b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p1:[0-9]+]] p1: ptr<u1b> [storage=static] = addr_of<ptr<u1b>>(%[[VALUE_v1]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: i2b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p2:[0-9]+]] p2: ptr<i2b> [storage=static] = addr_of<ptr<i2b>>(%[[VALUE_v2]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v11:[0-9]+]] v11: u11b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p11:[0-9]+]] p11: ptr<u11b> [storage=static] = addr_of<ptr<u11b>>(%[[VALUE_v11]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v12:[0-9]+]] v12: i12b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p12:[0-9]+]] p12: ptr<i12b> [storage=static] = addr_of<ptr<i12b>>(%[[VALUE_v12]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v21:[0-9]+]] v21: u21b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p21:[0-9]+]] p21: ptr<u21b> [storage=static] = addr_of<ptr<u21b>>(%[[VALUE_v21]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v22:[0-9]+]] v22: i22b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p22:[0-9]+]] p22: ptr<i22b> [storage=static] = addr_of<ptr<i22b>>(%[[VALUE_v22]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v31:[0-9]+]] v31: u31b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p31:[0-9]+]] p31: ptr<u31b> [storage=static] = addr_of<ptr<u31b>>(%[[VALUE_v31]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v32:[0-9]+]] v32: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p32:[0-9]+]] p32: ptr<i32b> [storage=static] = addr_of<ptr<i32b>>(%[[VALUE_v32]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v41:[0-9]+]] v41: u41b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p41:[0-9]+]] p41: ptr<u41b> [storage=static] = addr_of<ptr<u41b>>(%[[VALUE_v41]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v42:[0-9]+]] v42: i42b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p42:[0-9]+]] p42: ptr<i42b> [storage=static] = addr_of<ptr<i42b>>(%[[VALUE_v42]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v127:[0-9]+]] v127: u127b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p127:[0-9]+]] p127: ptr<u127b> [storage=static] = addr_of<ptr<u127b>>(%[[VALUE_v127]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v128:[0-9]+]] v128: i128b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p128:[0-9]+]] p128: ptr<i128b> [storage=static] = addr_of<ptr<i128b>>(%[[VALUE_v128]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v257:[0-9]+]] v257: u257b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p257:[0-9]+]] p257: ptr<u257b> [storage=static] = addr_of<ptr<u257b>>(%[[VALUE_v257]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v258:[0-9]+]] v258: i258b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p258:[0-9]+]] p258: ptr<i258b> [storage=static] = addr_of<ptr<i258b>>(%[[VALUE_v258]]) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
