// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct S6 { _BitInt(2) a : 1; };
struct S7 { char x; _BitInt(100) a : 63; _BitInt(128) b : 100; _BitInt(2) d : 1; char y; };
struct S8 { char x; _BitInt(100) : 0; char y; };
struct S9 { char x; _BitInt(100) v; };
struct D { char x; _BitInt(70) a : 60; _BitInt(70) b : 10; };
struct E { short s; char c; _BitInt(65) a : 65; };
struct LongLongStraddle { int a; char c; long long b : 40; };
struct LongLongFits { int a; char c; long long b : 24; char d; };
#if __BITINT_MAXWIDTH__ >= 575
struct S3 { char x; _BitInt(575) : 0; char y; };
struct S4 { char x; _BitInt(195) a : 63; _BitInt(282) b : 280; _BitInt(389) c : 23; _BitInt(2) d : 1; char y; };
struct S5 { char x; _BitInt(192) a : 191; unsigned _BitInt(192) b : 190; _BitInt(192) c : 189; char y; };
#endif
#if defined(__x86_64__)
static_assert(sizeof(struct S6) == 1 && alignof(struct S6) == 1);
static_assert(sizeof(struct S7) == 24 && alignof(struct S7) == 8);
static_assert(sizeof(struct S8) == 9 && alignof(struct S8) == 1);
static_assert(sizeof(struct S9) == 24 && alignof(struct S9) == 8);
static_assert(sizeof(struct D) == 16 && alignof(struct D) == 8);
static_assert(sizeof(struct E) == 16 && alignof(struct E) == 8);
static_assert(sizeof(struct LongLongStraddle) == 16 && alignof(struct LongLongStraddle) == 8);
static_assert(sizeof(struct LongLongFits) == 16 && alignof(struct LongLongFits) == 8);
static_assert(sizeof(_BitInt(9)) == 2 && alignof(_BitInt(9)) == 2);
static_assert(sizeof(_BitInt(33)) == 8 && alignof(_BitInt(33)) == 8);
static_assert(sizeof(_BitInt(65)) == 16 && alignof(_BitInt(65)) == 8);
static_assert(sizeof(_BitInt(128)) == 16 && alignof(_BitInt(128)) == 8);
#if __BITINT_MAXWIDTH__ >= 575
static_assert(sizeof(struct S3) == 9 && alignof(struct S3) == 1);
static_assert(sizeof(struct S4) == 48 && alignof(struct S4) == 8);
static_assert(sizeof(struct S5) == 88 && alignof(struct S5) == 8);
static_assert(sizeof(_BitInt(129)) == 24 && alignof(_BitInt(129)) == 8);
static_assert(sizeof(_BitInt(200)) == 32 && alignof(_BitInt(200)) == 8);
static_assert(sizeof(_BitInt(575)) == 72 && alignof(_BitInt(575)) == 8);
#endif
#elif defined(__i386__)
static_assert(sizeof(struct S6) == 1 && alignof(struct S6) == 1);
static_assert(sizeof(struct S7) == 24 && alignof(struct S7) == 4);
static_assert(sizeof(struct S8) == 5 && alignof(struct S8) == 1);
static_assert(sizeof(struct S9) == 20 && alignof(struct S9) == 4);
static_assert(sizeof(struct D) == 12 && alignof(struct D) == 4);
static_assert(sizeof(struct E) == 12 && alignof(struct E) == 4);
static_assert(sizeof(struct LongLongStraddle) == 12 && alignof(struct LongLongStraddle) == 4);
static_assert(sizeof(struct LongLongFits) == 12 && alignof(struct LongLongFits) == 4);
static_assert(sizeof(_BitInt(9)) == 2 && alignof(_BitInt(9)) == 2);
static_assert(sizeof(_BitInt(33)) == 8 && alignof(_BitInt(33)) == 4);
static_assert(sizeof(_BitInt(65)) == 12 && alignof(_BitInt(65)) == 4);
static_assert(sizeof(_BitInt(128)) == 16 && alignof(_BitInt(128)) == 4);
#if __BITINT_MAXWIDTH__ >= 575
static_assert(sizeof(struct S3) == 5 && alignof(struct S3) == 1);
static_assert(sizeof(struct S4) == 48 && alignof(struct S4) == 4);
static_assert(sizeof(struct S5) == 80 && alignof(struct S5) == 4);
static_assert(sizeof(_BitInt(129)) == 20 && alignof(_BitInt(129)) == 4);
static_assert(sizeof(_BitInt(200)) == 28 && alignof(_BitInt(200)) == 4);
static_assert(sizeof(_BitInt(575)) == 72 && alignof(_BitInt(575)) == 4);
#endif
#elif defined(__aarch64__)
static_assert(sizeof(struct S6) == 1 && alignof(struct S6) == 1);
static_assert(sizeof(struct S7) == 32 && alignof(struct S7) == 16);
static_assert(sizeof(struct S8) == 32 && alignof(struct S8) == 16);
static_assert(sizeof(struct S9) == 32 && alignof(struct S9) == 16);
static_assert(sizeof(struct D) == 16 && alignof(struct D) == 16);
static_assert(sizeof(struct E) == 16 && alignof(struct E) == 16);
static_assert(sizeof(struct LongLongStraddle) == 16 && alignof(struct LongLongStraddle) == 8);
static_assert(sizeof(struct LongLongFits) == 16 && alignof(struct LongLongFits) == 8);
static_assert(sizeof(_BitInt(9)) == 2 && alignof(_BitInt(9)) == 2);
static_assert(sizeof(_BitInt(33)) == 8 && alignof(_BitInt(33)) == 8);
static_assert(sizeof(_BitInt(65)) == 16 && alignof(_BitInt(65)) == 16);
static_assert(sizeof(_BitInt(128)) == 16 && alignof(_BitInt(128)) == 16);
#if __BITINT_MAXWIDTH__ >= 575
static_assert(sizeof(struct S3) == 32 && alignof(struct S3) == 16);
static_assert(sizeof(struct S4) == 48 && alignof(struct S4) == 16);
static_assert(sizeof(struct S5) == 96 && alignof(struct S5) == 16);
static_assert(sizeof(_BitInt(129)) == 32 && alignof(_BitInt(129)) == 16);
static_assert(sizeof(_BitInt(200)) == 32 && alignof(_BitInt(200)) == 16);
static_assert(sizeof(_BitInt(575)) == 80 && alignof(_BitInt(575)) == 16);
#endif
#endif

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_S6:[0-9]+]] S6 = struct {
// IR-NEXT:         field0 a: i2b : 1;
// IR-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_S7:[0-9]+]] S7 = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 a: i100b : 63;
// IR-NEXT:         field2 b: i128b : 100;
// IR-NEXT:         field3 d: i2b : 1;
// IR-NEXT:         field4 y: i8;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 1, 8, 21, 22], bit_offsets=[None, Some(8), Some(71), Some(171), None], bit_units=[(1, 21)], field_units=[None, Some(0), Some(0), Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S8:[0-9]+]] S8 = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 <anonymous>: i100b : 0;
// IR-NEXT:         field2 y: i8;
// IR-NEXT:     } [size=9, align=1, offsets=[0, 8, 8], bit_offsets=[None, Some(64), None]];
// IR-NEXT:     type @type[[TYPE_S9:[0-9]+]] S9 = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 v: i100b;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 a: i70b : 60;
// IR-NEXT:         field2 b: i70b : 10;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 1, 8], bit_offsets=[None, Some(8), Some(68)], bit_units=[(1, 9)], field_units=[None, Some(0), Some(0)]];
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = struct {
// IR-NEXT:         field0 s: i16;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:         field2 a: i65b : 65;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 2, 3], bit_offsets=[None, None, Some(24)], bit_units=[(3, 9)], field_units=[None, None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_LongLongStraddle:[0-9]+]] LongLongStraddle = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:         field2 b: i64 : 40;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 4, 8], bit_offsets=[None, None, Some(64)], bit_units=[(8, 5)], field_units=[None, None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_LongLongFits:[0-9]+]] LongLongFits = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:         field2 b: i64 : 24;
// IR-NEXT:         field3 d: i8;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 4, 5, 8], bit_offsets=[None, None, Some(40), None], bit_units=[(5, 3)], field_units=[None, None, Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S3:[0-9]+]] S3 = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 <anonymous>: i575b : 0;
// IR-NEXT:         field2 y: i8;
// IR-NEXT:     } [size=9, align=1, offsets=[0, 8, 8], bit_offsets=[None, Some(64), None]];
// IR-NEXT:     type @type[[TYPE_S4:[0-9]+]] S4 = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 a: i195b : 63;
// IR-NEXT:         field2 b: i282b : 280;
// IR-NEXT:         field3 c: i389b : 23;
// IR-NEXT:         field4 d: i2b : 1;
// IR-NEXT:         field5 y: i8;
// IR-NEXT:     } [size=48, align=8, offsets=[0, 1, 8, 43, 46, 47], bit_offsets=[None, Some(8), Some(71), Some(351), Some(374), None], bit_units=[(1, 46)], field_units=[None, Some(0), Some(0), Some(0), Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S5:[0-9]+]] S5 = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:         field1 a: i192b : 191;
// IR-NEXT:         field2 b: u192b : 190;
// IR-NEXT:         field3 c: i192b : 189;
// IR-NEXT:         field4 y: i8;
// IR-NEXT:     } [size=88, align=8, offsets=[0, 8, 32, 56, 80], bit_offsets=[None, Some(64), Some(256), Some(448), None], bit_units=[(8, 24), (32, 24), (56, 24)], field_units=[None, Some(0), Some(1), Some(2), None]];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
