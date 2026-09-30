// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct pair { int a; int b; };
struct odd_array { char a[3]; char b; };
struct bit_precise { _BitInt(24) a; float f; };
struct flexible { int n; int tail[]; };
struct zero_tail { float f; int tail[0]; };
struct aligned { int a; } __attribute__((aligned(16)));
struct empty {};
struct zero_only { int z[0]; };
struct empty_members { struct empty e[3]; int : 3; };

struct pair pair(struct pair value) { return value; }
struct odd_array odd_array(struct odd_array value) { return value; }
struct bit_precise bit_precise(struct bit_precise value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct zero_tail zero_tail(struct zero_tail value) { return value; }
struct aligned aligned(struct aligned value) { return value; }
struct empty empty(struct empty value) { return value; }
struct zero_only zero_only(struct zero_only value) { return value; }
struct empty_members empty_members(struct empty_members value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 4;
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
// IR-NEXT:     type @type[[TYPE_odd_array:[0-9]+]] odd_array = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:     } [size=4, align=1, offsets=[0, 3]];
// IR-NEXT:     type @type[[TYPE_bit_precise:[0-9]+]] bit_precise = struct {
// IR-NEXT:         field0 a: i24b;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_flexible:[0-9]+]] flexible = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_zero_tail:[0-9]+]] zero_tail = struct {
// IR-NEXT:         field0 f: f32;
// IR-NEXT:         field1 tail: array<i32, 0>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_aligned:[0-9]+]] aligned = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// IR-NEXT:     } [size=4, align=1, offsets=[]];
// IR-NEXT:     type @type[[TYPE_zero_only:[0-9]+]] zero_only = struct {
// IR-NEXT:         field0 z: array<i32, 0>;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_empty_members:[0-9]+]] empty_members = struct {
// IR-NEXT:         field0 e: array<@type[[TYPE_empty]], 3>;
// IR-NEXT:         field1 <anonymous>: i32 : 3;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 12], bit_offsets=[None, Some(96)], bit_units=[(12, 4)], field_units=[None, Some(0)]];
// IR-NEXT:     fn %[[VALUE_pair:[0-9]+]] @pair(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_odd_array:[0-9]+]] @odd_array(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE_odd_array]]) -> @type[[TYPE_odd_array]] [linkage=external] [abi=x86_win32(native_c) -> sret<align=1>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_odd_array]], reason=return>(read<@type[[TYPE_odd_array]]>(%[[VALUE_value_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_bit_precise:[0-9]+]] @bit_precise(%[[VALUE_value_3:[0-9]+]] value: @type[[TYPE_bit_precise]]) -> @type[[TYPE_bit_precise]] [linkage=external] [abi=x86_win32(native_c) -> sret<align=4>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_bit_precise]], reason=return>(read<@type[[TYPE_bit_precise]]>(%[[VALUE_value_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_flexible:[0-9]+]] @flexible(%[[VALUE_value_4:[0-9]+]] value: @type[[TYPE_flexible]]) -> @type[[TYPE_flexible]] [linkage=external] [abi=x86_win32(native_c) -> sret<align=4>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_flexible]], reason=return>(read<@type[[TYPE_flexible]]>(%[[VALUE_value_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_zero_tail:[0-9]+]] @zero_tail(%[[VALUE_value_5:[0-9]+]] value: @type[[TYPE_zero_tail]]) -> @type[[TYPE_zero_tail]] [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_zero_tail]], reason=return>(read<@type[[TYPE_zero_tail]]>(%[[VALUE_value_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_aligned:[0-9]+]] @aligned(%[[VALUE_value_6:[0-9]+]] value: @type[[TYPE_aligned]]) -> @type[[TYPE_aligned]] [linkage=external] [abi=x86_win32(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_aligned]], reason=return>(read<@type[[TYPE_aligned]]>(%[[VALUE_value_6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_empty:[0-9]+]] @empty(%[[VALUE_value_7:[0-9]+]] value: @type[[TYPE_empty]]) -> @type[[TYPE_empty]] [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_value_7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_zero_only:[0-9]+]] @zero_only(%[[VALUE_value_8:[0-9]+]] value: @type[[TYPE_zero_only]]) -> @type[[TYPE_zero_only]] [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_zero_only]], reason=return>(read<@type[[TYPE_zero_only]]>(%[[VALUE_value_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_empty_members:[0-9]+]] @empty_members(%[[VALUE_value_9:[0-9]+]] value: @type[[TYPE_empty_members]]) -> @type[[TYPE_empty_members]] [linkage=external] [abi=x86_win32(native_c) -> void] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_empty_members]], reason=return>(read<@type[[TYPE_empty_members]]>(%[[VALUE_value_9]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
