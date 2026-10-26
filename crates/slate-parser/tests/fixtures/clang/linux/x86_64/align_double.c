struct wide { char c; double d; long long l; unsigned long long u; };
struct extended { char c; long double x; };

int wide_size = sizeof(struct wide);
int wide_align = _Alignof(struct wide);
int extended_size = sizeof(struct extended);
int extended_align = _Alignof(struct extended);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ON
// SLATE-FILECHECK-PREFIX-ARGS ON -malign-double

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
// DEFAULT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:         field2 l: i64;
// DEFAULT-NEXT:         field3 u: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_extended:[0-9]+]] extended = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 x: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     global %[[VALUE_wide_size:[0-9]+]] wide_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(32))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_align:[0-9]+]] wide_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_extended_size:[0-9]+]] extended_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(32))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_extended_align:[0-9]+]] extended_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(16))) [linkage=external];
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
// ON-NEXT:         storage f80 [size=16, align=8];
// ON-NEXT:         storage f128 [size=16, align=16];
// ON-NEXT:         storage d32 [size=4, align=4];
// ON-NEXT:         storage d64 [size=8, align=8];
// ON-NEXT:         storage d128 [size=16, align=16];
// ON-NEXT:     }
// ON-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// ON-NEXT:         field0 c: i8;
// ON-NEXT:         field1 d: f64;
// ON-NEXT:         field2 l: i64;
// ON-NEXT:         field3 u: u64;
// ON-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// ON-NEXT:     type @type[[TYPE_extended:[0-9]+]] extended = struct {
// ON-NEXT:         field0 c: i8;
// ON-NEXT:         field1 x: f80;
// ON-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// ON-NEXT:     global %[[VALUE_wide_size:[0-9]+]] wide_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(32))) [linkage=external];
// ON-NEXT:     global %[[VALUE_wide_align:[0-9]+]] wide_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// ON-NEXT:     global %[[VALUE_extended_size:[0-9]+]] extended_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(24))) [linkage=external];
// ON-NEXT:     global %[[VALUE_extended_align:[0-9]+]] extended_align: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// ON-NEXT: }
// SLATE-FILECHECK-END ON
