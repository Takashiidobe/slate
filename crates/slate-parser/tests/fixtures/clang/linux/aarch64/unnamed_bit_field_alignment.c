// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
struct plain { char c; int : 4; };
struct __attribute__((packed)) packed { char c; int : 4; };
struct wide { char c; long long : 4; };
struct zero_width { char c; int : 0; char d; };
#pragma pack(4)
struct __attribute__((packed)) packed_pragma { char c; int : 4; };
struct pragma { char c; long long : 4; };
#pragma pack()
struct named_after { char c; int : 4; short s; };
union unnamed_union { int : 1; };
union named_union { int x : 1; };
union mixed_union { int : 20; char c; };
#pragma pack(1)
union pragma_union { int x : 20; };
#pragma pack()

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_plain:[0-9]+]] plain = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i32 : 4;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_packed:[0-9]+]] packed = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i32 : 4;
// IR-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i64 : 4;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_zero_width:[0-9]+]] zero_width = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i32 : 0;
// IR-NEXT:         field2 d: u8;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4, 4], bit_offsets=[None, Some(32), None]];
// IR-NEXT:     type @type[[TYPE_packed_pragma:[0-9]+]] packed_pragma = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i32 : 4;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_pragma:[0-9]+]] pragma = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i64 : 4;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type[[TYPE_named_after:[0-9]+]] named_after = struct {
// IR-NEXT:         field0 c: u8;
// IR-NEXT:         field1 <anonymous>: i32 : 4;
// IR-NEXT:         field2 s: i16;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1, 2], bit_offsets=[None, Some(8), None], bit_units=[(1, 1)], field_units=[None, Some(0), None]];
// IR-NEXT:     type @type[[TYPE_unnamed_union:[0-9]+]] unnamed_union = union {
// IR-NEXT:         field0 <anonymous>: i32 : 1;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_named_union:[0-9]+]] named_union = union {
// IR-NEXT:         field0 x: i32 : 1;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_mixed_union:[0-9]+]] mixed_union = union {
// IR-NEXT:         field0 <anonymous>: i32 : 20;
// IR-NEXT:         field1 c: u8;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 3)], field_units=[Some(0), None]];
// IR-NEXT:     type @type[[TYPE_pragma_union:[0-9]+]] pragma_union = union {
// IR-NEXT:         field0 x: i32 : 20;
// IR-NEXT:     } [size=3, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 3)], field_units=[Some(0)]];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
