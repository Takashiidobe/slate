// SLATE-FILECHECK-DEFINES TARGET
// SLATE-FILECHECK-ARGS --dump-ir-types

#if __SIZEOF_POINTER__ != 4
#error wrong target predefines
#endif
#if __SIZEOF_LONG__ != 4 || __SIZEOF_LONG_DOUBLE__ != 12 || defined(__CHAR_UNSIGNED__) != 0
#error wrong target data model macros
#endif

struct Scalars { char first; long second; long double real; char last; };
struct Integers { char first; long long wide; short small; };
struct Pointers { char first; int *pointer; char last; };
struct Arrays { char first; double values[2]; char last; };
union Choice { long double real; char bytes[3]; };
struct Bits { unsigned int first:3; unsigned int second:5; unsigned int :0; unsigned int third:1; char last; };
struct MixedBits { unsigned char first:3; unsigned int second:3; unsigned short third:3; };
struct ZeroWidth { char first; int :0; char last; };
union ZeroWidthUnion { char first; int :0; };
struct __attribute__((packed)) Packed { char first; int second; };
struct __attribute__((packed)) PackedZeroWidth { char first; int :0; char last; };

// SLATE-FILECHECK-BEGIN TARGET
// TARGET: module {
// TARGET-NEXT:     target "i686-unknown-linux-gnu" {
// TARGET-NEXT:         endian = little;
// TARGET-NEXT:         pointer [size=4, align=4];
// TARGET-NEXT:         stack_alignment = 16;
// TARGET-NEXT:         long_double = f80;
// TARGET-NEXT:         storage bool [size=1, align=1];
// TARGET-NEXT:         storage i8, u8 [size=1, align=1];
// TARGET-NEXT:         storage i16, u16 [size=2, align=2];
// TARGET-NEXT:         storage i32, u32 [size=4, align=4];
// TARGET-NEXT:         storage i64, u64 [size=8, align=4];
// TARGET-NEXT:         storage i128, u128 [size=16, align=16];
// TARGET-NEXT:         storage bf16 [size=2, align=2];
// TARGET-NEXT:         storage f16 [size=2, align=2];
// TARGET-NEXT:         storage f32 [size=4, align=4];
// TARGET-NEXT:         storage f64 [size=8, align=4];
// TARGET-NEXT:         storage f80 [size=12, align=4];
// TARGET-NEXT:         storage f128 [size=16, align=16];
// TARGET-NEXT:         storage d32 [size=4, align=4];
// TARGET-NEXT:         storage d64 [size=8, align=8];
// TARGET-NEXT:         storage d128 [size=16, align=16];
// TARGET-NEXT:     }
// TARGET-NEXT:     type @type0 Scalars = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 second: i32;
// TARGET-NEXT:         field2 real: f80;
// TARGET-NEXT:         field3 last: i8;
// TARGET-NEXT:     } [size=24, align=4, offsets=[0, 4, 8, 20]];
// TARGET-NEXT:     type @type1 Integers = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 wide: i64;
// TARGET-NEXT:         field2 small: i16;
// TARGET-NEXT:     } [size=16, align=4, offsets=[0, 4, 12]];
// TARGET-NEXT:     type @type2 Pointers = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 pointer: ptr<i32>;
// TARGET-NEXT:         field2 last: i8;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// TARGET-NEXT:     type @type3 Arrays = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 values: array<f64, 2>;
// TARGET-NEXT:         field2 last: i8;
// TARGET-NEXT:     } [size=24, align=4, offsets=[0, 4, 20]];
// TARGET-NEXT:     type @type4 Choice = union {
// TARGET-NEXT:         field0 real: f80;
// TARGET-NEXT:         field1 bytes: array<i8, 3>;
// TARGET-NEXT:     } [size=12, align=4, offsets=[0, 0]];
// TARGET-NEXT:     type @type5 Bits = struct {
// TARGET-NEXT:         field0 first: u32 : 3;
// TARGET-NEXT:         field1 second: u32 : 5;
// TARGET-NEXT:         field2 <anonymous>: u32 : 0;
// TARGET-NEXT:         field3 third: u32 : 1;
// TARGET-NEXT:         field4 last: i8;
// TARGET-NEXT:     } [size=8, align=4, offsets=[0, 0, 4, 4, 5], bit_offsets=[Some(0), Some(3), Some(32), Some(32), None], bit_units=[(0, 1), (4, 1)], field_units=[Some(0), Some(0), None, Some(1), None]];
// TARGET-NEXT:     type @type6 MixedBits = struct {
// TARGET-NEXT:         field0 first: u8 : 3;
// TARGET-NEXT:         field1 second: u32 : 3;
// TARGET-NEXT:         field2 third: u16 : 3;
// TARGET-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(6)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0)]];
// TARGET-NEXT:     type @type7 ZeroWidth = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 last: i8;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 4, 4], bit_offsets=[None, Some(32), None]];
// TARGET-NEXT:     type @type8 ZeroWidthUnion = union {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:     } [size=1, align=1, offsets=[0, 0], bit_offsets=[None, Some(0)]];
// TARGET-NEXT:     type @type9 Packed = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 second: i32;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// TARGET-NEXT:     type @type10 PackedZeroWidth = struct {
// TARGET-NEXT:         field0 first: i8;
// TARGET-NEXT:         field1 <anonymous>: i32 : 0;
// TARGET-NEXT:         field2 last: i8;
// TARGET-NEXT:     } [size=5, align=1, offsets=[0, 4, 4], bit_offsets=[None, Some(32), None]];
// TARGET-NEXT: }
// SLATE-FILECHECK-END TARGET
