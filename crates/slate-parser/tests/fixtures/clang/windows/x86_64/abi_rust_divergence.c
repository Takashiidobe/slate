// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct pair { int a; int b; };
struct flexible { int n; int tail[]; };
struct flexible_large { int n; int m; int k; int tail[]; };

struct pair pair(struct pair value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct flexible_large flexible_large(struct flexible_large value) { return value; }
__int128 wide(__int128 value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_flexible:[0-9]+]] flexible = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_flexible_large:[0-9]+]] flexible_large = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 m: i32;
// IR-NEXT:         field2 k: i32;
// IR-NEXT:         field3 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8, 12]];
// IR-NEXT:     fn %[[VALUE_pair:[0-9]+]] @pair(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=win64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_flexible:[0-9]+]] @flexible(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE_flexible]]) -> @type[[TYPE_flexible]] [linkage=external] [abi=win64(byref<align=4>) -> sret<align=4>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_flexible]], reason=return>(read<@type[[TYPE_flexible]]>(%[[VALUE_value_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_flexible_large:[0-9]+]] @flexible_large(%[[VALUE_value_3:[0-9]+]] value: @type[[TYPE_flexible_large]]) -> @type[[TYPE_flexible_large]] [linkage=external] [abi=win64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_flexible_large]], reason=return>(read<@type[[TYPE_flexible_large]]>(%[[VALUE_value_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_wide:[0-9]+]] @wide(%[[VALUE_value_4:[0-9]+]] value: i128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i128>(%[[VALUE_value_4]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
