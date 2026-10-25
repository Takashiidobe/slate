struct plain { char a : 3; int b : 4; char c; };
struct __attribute__((gcc_struct)) opted_out { char a : 3; int b : 4; char c; };
struct widening { int x : 3; long long y : 3; int z : 3; };

int plain_size = sizeof(struct plain);
int opted_out_size = sizeof(struct opted_out);
int widening_size = sizeof(struct widening);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ON
// SLATE-FILECHECK-PREFIX-ARGS ON -mms-bitfields
// SLATE-FILECHECK-DEFINES OFF
// SLATE-FILECHECK-PREFIX-ARGS OFF -mms-bitfields -mno-ms-bitfields

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
// DEFAULT-NEXT:     type @type[[TYPE_plain:[0-9]+]] plain = struct {
// DEFAULT-NEXT:         field0 a: i8 : 3;
// DEFAULT-NEXT:         field1 b: i32 : 4;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_opted_out:[0-9]+]] opted_out = struct {
// DEFAULT-NEXT:         field0 a: i8 : 3;
// DEFAULT-NEXT:         field1 b: i32 : 4;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_widening:[0-9]+]] widening = struct {
// DEFAULT-NEXT:         field0 x: i32 : 3;
// DEFAULT-NEXT:         field1 y: i64 : 3;
// DEFAULT-NEXT:         field2 z: i32 : 3;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(6)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_plain_size:[0-9]+]] plain_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_opted_out_size:[0-9]+]] opted_out_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_widening_size:[0-9]+]] widening_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ON
// ON: module {
// ON-NEXT:     target "x86_64-unknown-linux-gnu" {
// ON-NEXT:         endian = little;
// ON-NEXT:         pointer [size=8, align=8];
// ON-NEXT:         stack_alignment = 16;
// ON-NEXT:         long_double = f80;
// ON-NEXT:         storage bool [size=1, align=1];
// ON-NEXT:         storage i8, u8 [size=1, align=1];
// ON-NEXT:         storage i16, u16 [size=2, align=2];
// ON-NEXT:         storage i32, u32 [size=4, align=4];
// ON-NEXT:         storage i64, u64 [size=8, align=8];
// ON-NEXT:         storage i128, u128 [size=16, align=16];
// ON-NEXT:         storage bf16 [size=2, align=2];
// ON-NEXT:         storage f16 [size=2, align=2];
// ON-NEXT:         storage f32 [size=4, align=4];
// ON-NEXT:         storage f64 [size=8, align=8];
// ON-NEXT:         storage f80 [size=16, align=16];
// ON-NEXT:         storage f128 [size=16, align=16];
// ON-NEXT:         storage d32 [size=4, align=4];
// ON-NEXT:         storage d64 [size=8, align=8];
// ON-NEXT:         storage d128 [size=16, align=16];
// ON-NEXT:     }
// ON-NEXT:     type @type[[TYPE_plain:[0-9]+]] plain = struct {
// ON-NEXT:         field0 a: i8 : 3;
// ON-NEXT:         field1 b: i32 : 4;
// ON-NEXT:         field2 c: i8;
// ON-NEXT:     } [size=12, align=4, offsets=[0, 4, 8], bit_offsets=[Some(0), Some(32), None], bit_units=[(0, 1), (4, 4)], field_units=[Some(0), Some(1), None]];
// ON-NEXT:     type @type[[TYPE_opted_out:[0-9]+]] opted_out = struct {
// ON-NEXT:         field0 a: i8 : 3;
// ON-NEXT:         field1 b: i32 : 4;
// ON-NEXT:         field2 c: i8;
// ON-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// ON-NEXT:     type @type[[TYPE_widening:[0-9]+]] widening = struct {
// ON-NEXT:         field0 x: i32 : 3;
// ON-NEXT:         field1 y: i64 : 3;
// ON-NEXT:         field2 z: i32 : 3;
// ON-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), Some(64), Some(128)], bit_units=[(0, 4), (8, 8), (16, 4)], field_units=[Some(0), Some(1), Some(2)]];
// ON-NEXT:     global %[[VALUE_plain_size:[0-9]+]] plain_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// ON-NEXT:     global %[[VALUE_opted_out_size:[0-9]+]] opted_out_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// ON-NEXT:     global %[[VALUE_widening_size:[0-9]+]] widening_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(24))) [linkage=external];
// ON-NEXT: }
// SLATE-FILECHECK-END ON
// SLATE-FILECHECK-BEGIN OFF
// OFF: module {
// OFF-NEXT:     target "x86_64-unknown-linux-gnu" {
// OFF-NEXT:         endian = little;
// OFF-NEXT:         pointer [size=8, align=8];
// OFF-NEXT:         stack_alignment = 16;
// OFF-NEXT:         long_double = f80;
// OFF-NEXT:         storage bool [size=1, align=1];
// OFF-NEXT:         storage i8, u8 [size=1, align=1];
// OFF-NEXT:         storage i16, u16 [size=2, align=2];
// OFF-NEXT:         storage i32, u32 [size=4, align=4];
// OFF-NEXT:         storage i64, u64 [size=8, align=8];
// OFF-NEXT:         storage i128, u128 [size=16, align=16];
// OFF-NEXT:         storage bf16 [size=2, align=2];
// OFF-NEXT:         storage f16 [size=2, align=2];
// OFF-NEXT:         storage f32 [size=4, align=4];
// OFF-NEXT:         storage f64 [size=8, align=8];
// OFF-NEXT:         storage f80 [size=16, align=16];
// OFF-NEXT:         storage f128 [size=16, align=16];
// OFF-NEXT:         storage d32 [size=4, align=4];
// OFF-NEXT:         storage d64 [size=8, align=8];
// OFF-NEXT:         storage d128 [size=16, align=16];
// OFF-NEXT:     }
// OFF-NEXT:     type @type[[TYPE_plain:[0-9]+]] plain = struct {
// OFF-NEXT:         field0 a: i8 : 3;
// OFF-NEXT:         field1 b: i32 : 4;
// OFF-NEXT:         field2 c: i8;
// OFF-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// OFF-NEXT:     type @type[[TYPE_opted_out:[0-9]+]] opted_out = struct {
// OFF-NEXT:         field0 a: i8 : 3;
// OFF-NEXT:         field1 b: i32 : 4;
// OFF-NEXT:         field2 c: i8;
// OFF-NEXT:     } [size=4, align=4, offsets=[0, 0, 1], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None]];
// OFF-NEXT:     type @type[[TYPE_widening:[0-9]+]] widening = struct {
// OFF-NEXT:         field0 x: i32 : 3;
// OFF-NEXT:         field1 y: i64 : 3;
// OFF-NEXT:         field2 z: i32 : 3;
// OFF-NEXT:     } [size=8, align=8, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(3), Some(6)], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0)]];
// OFF-NEXT:     global %[[VALUE_plain_size:[0-9]+]] plain_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// OFF-NEXT:     global %[[VALUE_opted_out_size:[0-9]+]] opted_out_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// OFF-NEXT:     global %[[VALUE_widening_size:[0-9]+]] widening_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// OFF-NEXT: }
// SLATE-FILECHECK-END OFF
