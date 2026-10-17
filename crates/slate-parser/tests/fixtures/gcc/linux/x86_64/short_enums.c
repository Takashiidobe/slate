enum small { s0 = 1, s1 = 127 };
enum shorter { h0 = 0, h1 = 32768 };
enum negative { n0 = -1, n1 = 100 };
enum wide { w0 = -2147483647 - 1 };

int small_size = sizeof(enum small);
int shorter_size = sizeof(enum shorter);
int negative_size = sizeof(enum negative);
int wide_size = sizeof(enum wide);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES SHORT
// SLATE-FILECHECK-PREFIX-ARGS SHORT -fshort-enums

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
// DEFAULT-NEXT:     type @type[[TYPE_small:[0-9]+]] small = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_s0:[0-9]+]] s0 = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_s1:[0-9]+]] s1 = const<i32>(127);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_shorter:[0-9]+]] shorter = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_s0]] h0 = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_s1]] h1 = const<i32>(32768);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_negative:[0-9]+]] negative = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_s0]] n0 = const<i32>(-1);
// DEFAULT-NEXT:         %[[VALUE_s1]] n1 = const<i32>(100);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_s0]] w0 = const<i32>(-2147483648);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_small_size:[0-9]+]] small_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_shorter_size:[0-9]+]] shorter_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_negative_size:[0-9]+]] negative_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_size:[0-9]+]] wide_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SHORT
// SHORT: module {
// SHORT-NEXT:     target "x86_64-unknown-linux-gnu" {
// SHORT-NEXT:         endian = little;
// SHORT-NEXT:         pointer [size=8, align=8];
// SHORT-NEXT:         stack_alignment = 16;
// SHORT-NEXT:         long_double = f80;
// SHORT-NEXT:         storage bool [size=1, align=1];
// SHORT-NEXT:         storage i8, u8 [size=1, align=1];
// SHORT-NEXT:         storage i16, u16 [size=2, align=2];
// SHORT-NEXT:         storage i32, u32 [size=4, align=4];
// SHORT-NEXT:         storage i64, u64 [size=8, align=8];
// SHORT-NEXT:         storage i128, u128 [size=16, align=16];
// SHORT-NEXT:         storage bf16 [size=2, align=2];
// SHORT-NEXT:         storage f16 [size=2, align=2];
// SHORT-NEXT:         storage f32 [size=4, align=4];
// SHORT-NEXT:         storage f64 [size=8, align=8];
// SHORT-NEXT:         storage f80 [size=16, align=16];
// SHORT-NEXT:         storage f128 [size=16, align=16];
// SHORT-NEXT:         storage d32 [size=4, align=4];
// SHORT-NEXT:         storage d64 [size=8, align=8];
// SHORT-NEXT:         storage d128 [size=16, align=16];
// SHORT-NEXT:     }
// SHORT-NEXT:     type @type[[TYPE_small:[0-9]+]] small = enum : u8 {
// SHORT-NEXT:         %[[VALUE_s0:[0-9]+]] s0 = const<i32>(1);
// SHORT-NEXT:         %[[VALUE_s1:[0-9]+]] s1 = const<i32>(127);
// SHORT-NEXT:     } [size=1, align=1];
// SHORT-NEXT:     type @type[[TYPE_shorter:[0-9]+]] shorter = enum : u16 {
// SHORT-NEXT:         %[[VALUE_s0]] h0 = const<i32>(0);
// SHORT-NEXT:         %[[VALUE_s1]] h1 = const<i32>(32768);
// SHORT-NEXT:     } [size=2, align=2];
// SHORT-NEXT:     type @type[[TYPE_negative:[0-9]+]] negative = enum : i8 {
// SHORT-NEXT:         %[[VALUE_s0]] n0 = const<i32>(-1);
// SHORT-NEXT:         %[[VALUE_s1]] n1 = const<i32>(100);
// SHORT-NEXT:     } [size=1, align=1];
// SHORT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = enum : i32 {
// SHORT-NEXT:         %[[VALUE_s0]] w0 = const<i32>(-2147483648);
// SHORT-NEXT:     } [size=4, align=4];
// SHORT-NEXT:     global %[[VALUE_small_size:[0-9]+]] small_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(1))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_shorter_size:[0-9]+]] shorter_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_negative_size:[0-9]+]] negative_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(1))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wide_size:[0-9]+]] wide_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// SHORT-NEXT: }
// SLATE-FILECHECK-END SHORT
