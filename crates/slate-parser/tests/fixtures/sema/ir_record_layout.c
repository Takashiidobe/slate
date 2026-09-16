// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-ARGS --dump-ir-types

struct Plain { char a; int b; short c; };
union Choice { char a; int b; };
union BitChoice { unsigned int flag:1; char byte; };
union __attribute__((packed)) PackedBitChoice { unsigned int flag:1; char byte; };
struct __attribute__((packed)) Packed { char a; int b; };
struct __attribute__((packed, aligned(4))) PackedAligned { char a; int b; };
struct AlignedField { char a; int b __attribute__((aligned(8))); };
struct AlignAsField { char a; _Alignas(16) char b; char c; };
struct ZeroWidth { char a; int :0; char b; };
struct Bits { unsigned int a:3; unsigned int b:5; unsigned int :0; unsigned int c:1; char d; };
struct MixedBits { unsigned char a:3; unsigned int b:3; unsigned short c:3; };
struct Outer { char x; struct { short s; } inner; char y; };
enum Color { Red, Green = 4, Blue };
enum SignedColor { Negative = -1, Positive = Negative + 2 };
enum Wide { Big = 4294967295U };
enum Small : unsigned short { First = 1, Second = 2 };
enum __attribute__((aligned(8))) AlignedEnum { Single = 1 };
struct EnumHolder { char a; enum AlignedEnum value; char b; };
enum Opaque : unsigned char;
struct OpaqueHolder { enum Opaque value; };

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
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 Plain = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 Choice = union {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 BitChoice = union {
// DEFAULT-NEXT:         field0 flag: u32 : 1;
// DEFAULT-NEXT:         field1 byte: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type3 PackedBitChoice = union {
// DEFAULT-NEXT:         field0 flag: u32 : 1;
// DEFAULT-NEXT:         field1 byte: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type4 Packed = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type5 PackedAligned = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type6 AlignedField = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type7 AlignAsField = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16, 17]];
// DEFAULT-NEXT:     type @type8 ZeroWidth = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field2 b: i8;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 4, 4], bit_offsets=[None, Some(32), None]];
// DEFAULT-NEXT:     type @type9 Bits = struct {
// DEFAULT-NEXT:         field0 a: u32 : 3;
// DEFAULT-NEXT:         field1 b: u32 : 5;
// DEFAULT-NEXT:         field2 <anonymous>: u32 : 0;
// DEFAULT-NEXT:         field3 c: u32 : 1;
// DEFAULT-NEXT:         field4 d: i8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 4, 4, 5], bit_offsets=[Some(0), Some(3), Some(32), Some(32), None], bit_units=[(0, 1), (4, 1)], field_units=[Some(0), Some(0), None, Some(1), None]];
// DEFAULT-NEXT:     type @type10 MixedBits = struct {
// DEFAULT-NEXT:         field0 a: u8 : 3;
// DEFAULT-NEXT:         field1 b: u32 : 3;
// DEFAULT-NEXT:         field2 c: u16 : 3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(6)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type11 Outer = struct {
// DEFAULT-NEXT:         field0 x: i8;
// DEFAULT-NEXT:         field1 inner: @type12;
// DEFAULT-NEXT:         field2 y: i8;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 2, 4]];
// DEFAULT-NEXT:     type @type12 = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type13 Color = enum : i32 {
// DEFAULT-NEXT:         %0 Red = const<i32>(0);
// DEFAULT-NEXT:         %1 Green = const<i32>(4);
// DEFAULT-NEXT:         %2 Blue = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type14 SignedColor = enum : i32 {
// DEFAULT-NEXT:         %0 Negative = const<i32>(-1);
// DEFAULT-NEXT:         %1 Positive = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type15 Wide = enum : u32 {
// DEFAULT-NEXT:         %0 Big = const<u32>(4294967295);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type16 Small = enum : u16 {
// DEFAULT-NEXT:         %0 First = const<u16>(1);
// DEFAULT-NEXT:         %1 Second = const<u16>(2);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type17 AlignedEnum = enum : i32 {
// DEFAULT-NEXT:         %0 Single = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=8];
// DEFAULT-NEXT:     type @type18 EnumHolder = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 value: @type17;
// DEFAULT-NEXT:         field2 b: i8;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type19 Opaque = enum : u8 incomplete [size=1, align=1];
// DEFAULT-NEXT:     type @type20 OpaqueHolder = struct {
// DEFAULT-NEXT:         field0 value: @type19;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
