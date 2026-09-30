
typedef struct { int a,b,c,d; } Big;
typedef struct { int i; } Small;
typedef struct { short s; } Short;
typedef struct { } ZeroSized;

Big returnBig(Big x) { return x; }

Small returnSmall(Small x) { return x; }

Short returnShort(Short x) { return x; }

ZeroSized returnZero(ZeroSized x) { return x; }

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_Big:[0-9]+]] Big = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Small:[0-9]+]] Small = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Short:[0-9]+]] Short = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_ZeroSized:[0-9]+]] ZeroSized = @type[[TYPE3]];
// DEFAULT-NEXT:     fn %[[VALUE_returnBig:[0-9]+]] @returnBig(%[[VALUE_x:[0-9]+]] x: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_returnSmall:[0-9]+]] @returnSmall(%[[VALUE_x_2:[0-9]+]] x: @type[[TYPE1]]) -> @type[[TYPE1]] [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE1]], reason=return>(read<@type[[TYPE1]]>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_returnShort:[0-9]+]] @returnShort(%[[VALUE_x_3:[0-9]+]] x: @type[[TYPE2]]) -> @type[[TYPE2]] [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE2]], reason=return>(read<@type[[TYPE2]]>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_returnZero:[0-9]+]] @returnZero(%[[VALUE_x_4:[0-9]+]] x: @type[[TYPE3]]) -> @type[[TYPE3]] [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE3]], reason=return>(read<@type[[TYPE3]]>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
