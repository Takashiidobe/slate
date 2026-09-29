// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/9799 */
/* Verify that GCC doesn't crash on excess elements
   in initializer for a flexible array member.  */

typedef struct {
    int aaa;
} s1_t;

typedef struct {
    int bbb;
    s1_t s1_array[];
} s2_t;

static s2_t s2_array[]= {
    { 1, 4 },	/* { dg-error "(initialization of flexible array member|near)" } */
    { 2, 5 },	/* { dg-error "(initialization of flexible array member|near)" } */
    { 3, 6 }	/* { dg-error "(initialization of flexible array member|near)" } */
};

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 aaa: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s1_t:[0-9]+]] s1_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 bbb: i32;
// DEFAULT-NEXT:         field1 s1_array: array<@type[[TYPE0]], incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_s2_t:[0-9]+]] s2_t = @type[[TYPE1]];
// DEFAULT-NEXT:     global %[[VALUE_s2_array:[0-9]+]] s2_array: array<@type[[TYPE1]], 3> [storage=static] = aggregate<array<@type[[TYPE1]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(1), field1 = aggregate<array<@type[[TYPE0]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(4)))), index1 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(2), field1 = aggregate<array<@type[[TYPE0]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(5)))), index2 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<array<@type[[TYPE0]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(6))))) [linkage=internal];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
