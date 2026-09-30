// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23

#define MS __attribute__((ms_struct))
struct MS S1 { int a:3; char c; };
struct MS S2 { char a:3; int b:3; char c:3; };
struct MS S3 { int a:30; int b:4; };
struct MS S4 { int a:3; int :0; int b:3; };
struct MS S5 { char c; int :0; char d; };
struct MS S6 { char a:3; long long :0; char d; };
union MS U7 { int a:3; char b; };
union MS U8 { int a:3; };
struct __attribute__((packed, ms_struct)) S9 { char c; int a:3; };
#pragma pack(1)
struct MS S10 { char c; int a:3; };
#pragma pack()
struct MS S11 { char c; int a:3 __attribute__((aligned(8))); };
struct MS S12 { char c; int :3; char d; };
struct MS S13 { short a:3; long long b:40; short c:5; };
enum E { X };
struct MS S14 { enum E a:3; unsigned b:3; };
struct MS S15 { _Bool a:1; _Bool b:1; int c:1; };
#pragma ms_struct on
struct __attribute__((gcc_struct)) S16 { int a:3; char c; };
struct S17 { int a:3; char c; };
#pragma ms_struct off
struct MS S18 { long double x; char c; };
struct MS S19 { char c; int a:3; int f[]; };
struct MS S20 { char c; char a:3; };
struct MS S21 { int a:3; char :0; char b; };
union MS U22 { int a:3; char :0; };
struct MS S23 { char c; int a:3; } __attribute__((aligned(16)));
struct MS S24 { char c; long long :0; };
struct MS S25 { unsigned a:3; int b:3; };
#pragma pack(1)
struct MS S26 { char c; int a:3; int :0; char d; };
struct MS S27 { char c; short a:3; int :0; char d; long long e:5; };
#pragma pack()
struct MS S28 { char a:3; char b:6; short c:9; short d:8; };
struct MS S29 { int a:3; struct { char x; } s; int b:3; };
typedef long long LL;
struct MS S30 { char c; LL a; LL b:3; };

int read_units(struct S2 *s2, struct S13 *s13) { return s2->b + s13->c; }

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
// IR-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = struct {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), None], bit_units=[(0, 4)], field_units=[Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S2:[0-9]+]] S2 = struct {
// IR-NEXT:         field0 a: i8 : 3;
// IR-NEXT:         field1 b: i32 : 3;
// IR-NEXT:         field2 c: i8 : 3;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[Some(0), Some(32), Some(64)], bit_units=[(0, 1), (4, 4), (8, 1)], field_units=[Some(0), Some(1), Some(2)]];
// IR-NEXT:     type @type[[TYPE_S3:[0-9]+]] S3 = struct {
// IR-NEXT:         field0 a: i32 : 30;
// IR-NEXT:         field1 b: i32 : 4;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), Some(1)]];
// IR-NEXT:     type @type[[TYPE_S4:[0-9]+]] S4 = struct {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 <anonymous>: i32 : 0;
// IR-NEXT:         field2 b: i32 : 3;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), Some(32)], bit_units=[(0, 4), (4, 4)], field_units=[Some(0), None, Some(1)]];
// IR-NEXT:     type @type[[TYPE_S5:[0-9]+]] S5 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 <anonymous>: i32 : 0;
// IR-NEXT:         field2 d: i8;
// IR-NEXT:     } [size=2, align=1, offsets=[0, 1, 1], bit_offsets=[None, Some(8), None]];
// IR-NEXT:     type @type[[TYPE_S6:[0-9]+]] S6 = struct {
// IR-NEXT:         field0 a: i8 : 3;
// IR-NEXT:         field1 <anonymous>: i64 : 0;
// IR-NEXT:         field2 d: i8;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8, 8], bit_offsets=[Some(0), Some(64), None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// IR-NEXT:     type @type[[TYPE_U7:[0-9]+]] U7 = union {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 4)], field_units=[Some(0), None]];
// IR-NEXT:     type @type[[TYPE_U8:[0-9]+]] U8 = union {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:     } [size=4, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 4)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_S9:[0-9]+]] S9 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i32 : 3;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_S10:[0-9]+]] S10 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i32 : 3;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 4)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_S11:[0-9]+]] S11 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i32 : 3;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 4)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_S12:[0-9]+]] S12 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 <anonymous>: i32 : 3;
// IR-NEXT:         field2 d: i8;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[None, Some(32), None], bit_units=[(4, 4)], field_units=[None, Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S13:[0-9]+]] S13 = struct {
// IR-NEXT:         field0 a: i16 : 3;
// IR-NEXT:         field1 b: i64 : 40;
// IR-NEXT:         field2 c: i16 : 5;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), Some(64), Some(128)], bit_units=[(0, 2), (8, 8), (16, 2)], field_units=[Some(0), Some(1), Some(2)]];
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_X:[0-9]+]] X = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_S14:[0-9]+]] S14 = struct {
// IR-NEXT:         field0 a: @type[[TYPE_E]] : 3;
// IR-NEXT:         field1 b: u32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// IR-NEXT:     type @type[[TYPE_S15:[0-9]+]] S15 = struct {
// IR-NEXT:         field0 a: bool : 1;
// IR-NEXT:         field1 b: bool : 1;
// IR-NEXT:         field2 c: i32 : 1;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 0, 4], bit_offsets=[Some(0), Some(1), Some(32)], bit_units=[(0, 1), (4, 4)], field_units=[Some(0), Some(0), Some(1)]];
// IR-NEXT:     type @type[[TYPE_S16:[0-9]+]] S16 = struct {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S17:[0-9]+]] S17 = struct {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), None], bit_units=[(0, 4)], field_units=[Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S18:[0-9]+]] S18 = struct {
// IR-NEXT:         field0 x: f80;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type[[TYPE_S19:[0-9]+]] S19 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i32 : 3;
// IR-NEXT:         field2 f: array<i32, incomplete>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4, 8], bit_offsets=[None, Some(32), None], bit_units=[(4, 4)], field_units=[None, Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S20:[0-9]+]] S20 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i8 : 3;
// IR-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_S21:[0-9]+]] S21 = struct {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 <anonymous>: i8 : 0;
// IR-NEXT:         field2 b: i8;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[Some(0), Some(32), None], bit_units=[(0, 4)], field_units=[Some(0), None, None]];
// IR-NEXT:     type @type[[TYPE_U22:[0-9]+]] U22 = union {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 <anonymous>: i8 : 0;
// IR-NEXT:     } [size=4, align=1, offsets=[0, 0], bit_offsets=[Some(0), Some(0)], bit_units=[(0, 4)], field_units=[Some(0), None]];
// IR-NEXT:     type @type[[TYPE_S23:[0-9]+]] S23 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i32 : 3;
// IR-NEXT:     } [size=16, align=16, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_S24:[0-9]+]] S24 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 <anonymous>: i64 : 0;
// IR-NEXT:     } [size=1, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)]];
// IR-NEXT:     type @type[[TYPE_S25:[0-9]+]] S25 = struct {
// IR-NEXT:         field0 a: u32 : 3;
// IR-NEXT:         field1 b: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// IR-NEXT:     type @type[[TYPE_S26:[0-9]+]] S26 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i32 : 3;
// IR-NEXT:         field2 <anonymous>: i32 : 0;
// IR-NEXT:         field3 d: i8;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 1, 4, 4], bit_offsets=[None, Some(8), Some(32), None], bit_units=[(1, 4)], field_units=[None, Some(0), None, None]];
// IR-NEXT:     type @type[[TYPE_S27:[0-9]+]] S27 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i16 : 3;
// IR-NEXT:         field2 <anonymous>: i32 : 0;
// IR-NEXT:         field3 d: i8;
// IR-NEXT:         field4 e: i64 : 5;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 1, 4, 4, 5], bit_offsets=[None, Some(8), Some(32), None, Some(40)], bit_units=[(1, 2), (5, 8)], field_units=[None, Some(0), None, None, Some(1)]];
// IR-NEXT:     type @type[[TYPE_S28:[0-9]+]] S28 = struct {
// IR-NEXT:         field0 a: i8 : 3;
// IR-NEXT:         field1 b: i8 : 6;
// IR-NEXT:         field2 c: i16 : 9;
// IR-NEXT:         field3 d: i16 : 8;
// IR-NEXT:     } [size=6, align=2, offsets=[0, 1, 2, 4], bit_offsets=[Some(0), Some(8), Some(16), Some(32)], bit_units=[(0, 1), (1, 1), (2, 2), (4, 2)], field_units=[Some(0), Some(1), Some(2), Some(3)]];
// IR-NEXT:     type @type[[TYPE_S29:[0-9]+]] S29 = struct {
// IR-NEXT:         field0 a: i32 : 3;
// IR-NEXT:         field1 s: @type[[TYPE0:[0-9]+]];
// IR-NEXT:         field2 b: i32 : 3;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[Some(0), None, Some(64)], bit_units=[(0, 4), (8, 4)], field_units=[Some(0), None, Some(1)]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 x: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_LL:[0-9]+]] LL = i64;
// IR-NEXT:     type @type[[TYPE_S30:[0-9]+]] S30 = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 a: i64;
// IR-NEXT:         field2 b: i64 : 3;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[None, None, Some(128)], bit_units=[(16, 8)], field_units=[None, None, Some(0)]];
// IR-NEXT:     fn %[[VALUE_read_units:[0-9]+]] @read_units(%[[VALUE_s2:[0-9]+]] s2: ptr<@type[[TYPE_S2]]>, %[[VALUE_s13:[0-9]+]] s13: ptr<@type[[TYPE_S13]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(bitfield1<unit=1, bytes=4..8, bits=0..3>(deref(read<ptr<@type[[TYPE_S2]]>>(%[[VALUE_s2]])))), widen<i32, reason=promotion>(read<i16>(bitfield2<unit=2, bytes=16..18, bits=0..5>(deref(read<ptr<@type[[TYPE_S13]]>>(%[[VALUE_s13]]))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
