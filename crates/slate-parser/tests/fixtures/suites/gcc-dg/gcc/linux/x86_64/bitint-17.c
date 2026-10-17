/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)

struct S1 { char x; char : 0; char y; };
struct S2 { char x; int : 0; char y; };
#if __BITINT_MAXWIDTH__ >= 575
struct S3 { char x; _BitInt(575) : 0; char y; };
#endif
#if __BITINT_MAXWIDTH__ >= 389
struct S4 { char x; _BitInt(195) a : 63; _BitInt(282) b : 280; _BitInt(389) c : 23; _BitInt(2) d : 1; char y; };
#endif
#if __BITINT_MAXWIDTH__ >= 192
struct S5 { char x; _BitInt(192) a : 191; unsigned _BitInt(192) b : 190; _BitInt(192) c : 189; char y; };
#endif
struct S6 { _BitInt(2) a : 1; };
#if __BITINT_MAXWIDTH__ >= 389
struct S4 s4;
static_assert (expr_has_type (s4.a + 1uwb, _BitInt(195)));
static_assert (expr_has_type (s4.b + 1uwb, _BitInt(282)));
static_assert (expr_has_type (s4.c + 1uwb, _BitInt(389)));
static_assert (expr_has_type (s4.d * 0wb, _BitInt(2)));
#endif
#if __BITINT_MAXWIDTH__ >= 192
struct S5 s5;
static_assert (expr_has_type (s5.a + 1uwb, _BitInt(192)));
static_assert (expr_has_type (s5.b + 1wb, unsigned _BitInt(192)));
static_assert (expr_has_type (s5.c + 1uwb, _BitInt(192)));
#endif
struct S6 s6;
static_assert (expr_has_type (s6.a + 0wb, _BitInt(2)));
#if defined(__x86_64__) && __LP64__ && __BITINT_MAXWIDTH__ >= 575
static_assert (sizeof (struct S1) == 2);
static_assert (sizeof (struct S2) == 5);
static_assert (sizeof (struct S3) == 9);
static_assert (sizeof (struct S4) == 48);
static_assert (sizeof (struct S5) == 88);
static_assert (sizeof (struct S6) == 1);
static_assert (alignof (struct S1) == 1);
static_assert (alignof (struct S2) == 1);
static_assert (alignof (struct S3) == 1);
static_assert (alignof (struct S4) == 8);
static_assert (alignof (struct S5) == 8);
static_assert (alignof (struct S6) == 1);
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
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i8 : 0;
// DEFAULT-NEXT:         field2 y: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[None, Some(8), None]];
// DEFAULT-NEXT:     type @type[[TYPE_S2:[0-9]+]] S2 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field2 y: i8;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 4, 4], bit_offsets=[None, Some(32), None]];
// DEFAULT-NEXT:     type @type[[TYPE_S3:[0-9]+]] S3 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i575b : 0;
// DEFAULT-NEXT:         field2 y: i8;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 8, 8], bit_offsets=[None, Some(64), None]];
// DEFAULT-NEXT:     type @type[[TYPE_S4:[0-9]+]] S4 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i195b : 63;
// DEFAULT-NEXT:         field2 b: i282b : 280;
// DEFAULT-NEXT:         field3 c: i389b : 23;
// DEFAULT-NEXT:         field4 d: i2b : 1;
// DEFAULT-NEXT:         field5 y: i8;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 1, 8, 43, 46, 47], bit_offsets=[None, Some(8), Some(71), Some(351), Some(374), None], bit_units=[(1, 46)], field_units=[None, Some(0), Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_S5:[0-9]+]] S5 = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 a: i192b : 191;
// DEFAULT-NEXT:         field2 b: u192b : 190;
// DEFAULT-NEXT:         field3 c: i192b : 189;
// DEFAULT-NEXT:         field4 y: i8;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 8, 32, 56, 80], bit_offsets=[None, Some(64), Some(256), Some(448), None], bit_units=[(8, 24), (32, 24), (56, 24)], field_units=[None, Some(0), Some(1), Some(2), None]];
// DEFAULT-NEXT:     type @type[[TYPE_S6:[0-9]+]] S6 = struct {
// DEFAULT-NEXT:         field0 a: i2b : 1;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_s4:[0-9]+]] s4: @type[[TYPE_S4]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s5:[0-9]+]] s5: @type[[TYPE_S5]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s6:[0-9]+]] s6: @type[[TYPE_S6]] [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
