struct empty {};
struct three { char a[3]; };
struct four { short a; char b; };
struct eight { int a, b; };
struct single_float { float f; };
struct nested_double { struct { double d[1]; } inner; };
union word { int i; char c; };

struct empty ret_empty(void) { struct empty r; return r; }
struct three ret_three(void) { struct three r = {{1, 2, 3}}; return r; }
struct four ret_four(void) { struct four r = {1, 2}; return r; }
struct eight ret_eight(void) { struct eight r = {1, 2}; return r; }
struct single_float ret_single_float(void) { struct single_float r = {1.5f}; return r; }
struct nested_double ret_nested_double(void) { struct nested_double r = {{{2.5}}}; return r; }
union word ret_word(void) { union word r = {7}; return r; }
int plain(int a, int b) { return a + b; }
int variadic(int a, ...) { return a; }
int __attribute__((cdecl)) explicit_cdecl(int a) { return a; }
int main(int argc, char **argv) { return argc + (argv != 0); }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES REG
// SLATE-FILECHECK-PREFIX-ARGS REG -freg-struct-return
// SLATE-FILECHECK-DEFINES PCC
// SLATE-FILECHECK-PREFIX-ARGS PCC -freg-struct-return -fpcc-struct-return
// SLATE-FILECHECK-DEFINES RTD
// SLATE-FILECHECK-PREFIX-ARGS RTD -mrtd
// SLATE-FILECHECK-DEFINES NORTD
// SLATE-FILECHECK-PREFIX-ARGS NORTD -mrtd -mno-rtd

// SLATE-FILECHECK-BEGIN REG
// REG: module {
// REG-NEXT:     target "i686-unknown-linux-gnu" {
// REG-NEXT:         endian = little;
// REG-NEXT:         pointer [size=4, align=4];
// REG-NEXT:         stack_alignment = 16;
// REG-NEXT:         long_double = f80;
// REG-NEXT:         storage bool [size=1, align=1];
// REG-NEXT:         storage i8, u8 [size=1, align=1];
// REG-NEXT:         storage i16, u16 [size=2, align=2];
// REG-NEXT:         storage i32, u32 [size=4, align=4];
// REG-NEXT:         storage i64, u64 [size=8, align=4];
// REG-NEXT:         storage i128, u128 [size=16, align=16];
// REG-NEXT:         storage bf16 [size=2, align=2];
// REG-NEXT:         storage f16 [size=2, align=2];
// REG-NEXT:         storage f32 [size=4, align=4];
// REG-NEXT:         storage f64 [size=8, align=4];
// REG-NEXT:         storage f80 [size=12, align=4];
// REG-NEXT:         storage f128 [size=16, align=16];
// REG-NEXT:         storage d32 [size=4, align=4];
// REG-NEXT:         storage d64 [size=8, align=8];
// REG-NEXT:         storage d128 [size=16, align=16];
// REG-NEXT:     }
// REG-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// REG-NEXT:     } [size=0, align=1, offsets=[]];
// REG-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// REG-NEXT:         field0 a: array<i8, 3>;
// REG-NEXT:     } [size=3, align=1, offsets=[0]];
// REG-NEXT:     type @type[[TYPE_four:[0-9]+]] four = struct {
// REG-NEXT:         field0 a: i16;
// REG-NEXT:         field1 b: i8;
// REG-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// REG-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// REG-NEXT:         field0 a: i32;
// REG-NEXT:         field1 b: i32;
// REG-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// REG-NEXT:     type @type[[TYPE_single_float:[0-9]+]] single_float = struct {
// REG-NEXT:         field0 f: f32;
// REG-NEXT:     } [size=4, align=4, offsets=[0]];
// REG-NEXT:     type @type[[TYPE_nested_double:[0-9]+]] nested_double = struct {
// REG-NEXT:         field0 inner: @type[[TYPE0:[0-9]+]];
// REG-NEXT:     } [size=8, align=4, offsets=[0]];
// REG-NEXT:     type @type[[TYPE0]] = struct {
// REG-NEXT:         field0 d: array<f64, 1>;
// REG-NEXT:     } [size=8, align=4, offsets=[0]];
// REG-NEXT:     type @type[[TYPE_word:[0-9]+]] word = union {
// REG-NEXT:         field0 i: i32;
// REG-NEXT:         field1 c: i8;
// REG-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// REG-NEXT:     fn %[[VALUE_ret_empty:[0-9]+]] @ret_empty() -> @type[[TYPE_empty]] [linkage=external] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_empty]] [storage=automatic];
// REG-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_r]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_ret_three:[0-9]+]] @ret_three() -> @type[[TYPE_three]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_three]] [storage=automatic] = aggregate<@type[[TYPE_three]], zero_fill=false>(field0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8>(const<i32>(1)), index1 = truncate<i8>(const<i32>(2)), index2 = truncate<i8>(const<i32>(3))));
// REG-NEXT:         return copy<@type[[TYPE_three]], reason=return>(read<@type[[TYPE_three]]>(%[[VALUE_r_2]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_ret_four:[0-9]+]] @ret_four() -> @type[[TYPE_four]] [linkage=external] [abi=x86_cdecl() -> coerce<i32>] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_four]] [storage=automatic] = aggregate<@type[[TYPE_four]], zero_fill=false>(field0 = truncate<i16>(const<i32>(1)), field1 = truncate<i8>(const<i32>(2)));
// REG-NEXT:         return copy<@type[[TYPE_four]], reason=return>(read<@type[[TYPE_four]]>(%[[VALUE_r_3]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_ret_eight:[0-9]+]] @ret_eight() -> @type[[TYPE_eight]] [linkage=external] [abi=x86_cdecl() -> coerce<i64>] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: @type[[TYPE_eight]] [storage=automatic] = aggregate<@type[[TYPE_eight]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// REG-NEXT:         return copy<@type[[TYPE_eight]], reason=return>(read<@type[[TYPE_eight]]>(%[[VALUE_r_4]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_ret_single_float:[0-9]+]] @ret_single_float() -> @type[[TYPE_single_float]] [linkage=external] [abi=x86_cdecl() -> coerce<f32>] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: @type[[TYPE_single_float]] [storage=automatic] = aggregate<@type[[TYPE_single_float]], zero_fill=false>(field0 = const<f32>(1.5));
// REG-NEXT:         return copy<@type[[TYPE_single_float]], reason=return>(read<@type[[TYPE_single_float]]>(%[[VALUE_r_5]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_ret_nested_double:[0-9]+]] @ret_nested_double() -> @type[[TYPE_nested_double]] [linkage=external] [abi=x86_cdecl() -> coerce<f64>] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: @type[[TYPE_nested_double]] [storage=automatic] = aggregate<@type[[TYPE_nested_double]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<array<f64, 1>, zero_fill=false>(index0 = const<f64>(2.5))));
// REG-NEXT:         return copy<@type[[TYPE_nested_double]], reason=return>(read<@type[[TYPE_nested_double]]>(%[[VALUE_r_6]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_ret_word:[0-9]+]] @ret_word() -> @type[[TYPE_word]] [linkage=external] [abi=x86_cdecl() -> coerce<i32>] [fallthrough=ub_if_used] {
// REG-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: @type[[TYPE_word]] [storage=automatic] = aggregate<@type[[TYPE_word]], zero_fill=false>(field0 = const<i32>(7));
// REG-NEXT:         return copy<@type[[TYPE_word]], reason=return>(read<@type[[TYPE_word]]>(%[[VALUE_r_7]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// REG-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// REG-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// REG-NEXT:         return read<i32>(%[[VALUE_a_3]]);
// REG-NEXT:     }
// REG-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// REG-NEXT:         return add<i32>(read<i32>(%[[VALUE_argc]]), from_bool<i32>(ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<ptr<i8>>>)));
// REG-NEXT:     }
// REG-NEXT: }
// SLATE-FILECHECK-END REG
// SLATE-FILECHECK-BEGIN PCC
// PCC: module {
// PCC-NEXT:     target "i686-unknown-linux-gnu" {
// PCC-NEXT:         endian = little;
// PCC-NEXT:         pointer [size=4, align=4];
// PCC-NEXT:         stack_alignment = 16;
// PCC-NEXT:         long_double = f80;
// PCC-NEXT:         storage bool [size=1, align=1];
// PCC-NEXT:         storage i8, u8 [size=1, align=1];
// PCC-NEXT:         storage i16, u16 [size=2, align=2];
// PCC-NEXT:         storage i32, u32 [size=4, align=4];
// PCC-NEXT:         storage i64, u64 [size=8, align=4];
// PCC-NEXT:         storage i128, u128 [size=16, align=16];
// PCC-NEXT:         storage bf16 [size=2, align=2];
// PCC-NEXT:         storage f16 [size=2, align=2];
// PCC-NEXT:         storage f32 [size=4, align=4];
// PCC-NEXT:         storage f64 [size=8, align=4];
// PCC-NEXT:         storage f80 [size=12, align=4];
// PCC-NEXT:         storage f128 [size=16, align=16];
// PCC-NEXT:         storage d32 [size=4, align=4];
// PCC-NEXT:         storage d64 [size=8, align=8];
// PCC-NEXT:         storage d128 [size=16, align=16];
// PCC-NEXT:     }
// PCC-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// PCC-NEXT:     } [size=0, align=1, offsets=[]];
// PCC-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// PCC-NEXT:         field0 a: array<i8, 3>;
// PCC-NEXT:     } [size=3, align=1, offsets=[0]];
// PCC-NEXT:     type @type[[TYPE_four:[0-9]+]] four = struct {
// PCC-NEXT:         field0 a: i16;
// PCC-NEXT:         field1 b: i8;
// PCC-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// PCC-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// PCC-NEXT:         field0 a: i32;
// PCC-NEXT:         field1 b: i32;
// PCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// PCC-NEXT:     type @type[[TYPE_single_float:[0-9]+]] single_float = struct {
// PCC-NEXT:         field0 f: f32;
// PCC-NEXT:     } [size=4, align=4, offsets=[0]];
// PCC-NEXT:     type @type[[TYPE_nested_double:[0-9]+]] nested_double = struct {
// PCC-NEXT:         field0 inner: @type[[TYPE0:[0-9]+]];
// PCC-NEXT:     } [size=8, align=4, offsets=[0]];
// PCC-NEXT:     type @type[[TYPE0]] = struct {
// PCC-NEXT:         field0 d: array<f64, 1>;
// PCC-NEXT:     } [size=8, align=4, offsets=[0]];
// PCC-NEXT:     type @type[[TYPE_word:[0-9]+]] word = union {
// PCC-NEXT:         field0 i: i32;
// PCC-NEXT:         field1 c: i8;
// PCC-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// PCC-NEXT:     fn %[[VALUE_ret_empty:[0-9]+]] @ret_empty() -> @type[[TYPE_empty]] [linkage=external] [abi=x86_cdecl() -> sret<align=1>] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_empty]] [storage=automatic];
// PCC-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_r]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_three:[0-9]+]] @ret_three() -> @type[[TYPE_three]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_three]] [storage=automatic] = aggregate<@type[[TYPE_three]], zero_fill=false>(field0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8>(const<i32>(1)), index1 = truncate<i8>(const<i32>(2)), index2 = truncate<i8>(const<i32>(3))));
// PCC-NEXT:         return copy<@type[[TYPE_three]], reason=return>(read<@type[[TYPE_three]]>(%[[VALUE_r_2]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_four:[0-9]+]] @ret_four() -> @type[[TYPE_four]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_four]] [storage=automatic] = aggregate<@type[[TYPE_four]], zero_fill=false>(field0 = truncate<i16>(const<i32>(1)), field1 = truncate<i8>(const<i32>(2)));
// PCC-NEXT:         return copy<@type[[TYPE_four]], reason=return>(read<@type[[TYPE_four]]>(%[[VALUE_r_3]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_eight:[0-9]+]] @ret_eight() -> @type[[TYPE_eight]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: @type[[TYPE_eight]] [storage=automatic] = aggregate<@type[[TYPE_eight]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// PCC-NEXT:         return copy<@type[[TYPE_eight]], reason=return>(read<@type[[TYPE_eight]]>(%[[VALUE_r_4]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_single_float:[0-9]+]] @ret_single_float() -> @type[[TYPE_single_float]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: @type[[TYPE_single_float]] [storage=automatic] = aggregate<@type[[TYPE_single_float]], zero_fill=false>(field0 = const<f32>(1.5));
// PCC-NEXT:         return copy<@type[[TYPE_single_float]], reason=return>(read<@type[[TYPE_single_float]]>(%[[VALUE_r_5]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_nested_double:[0-9]+]] @ret_nested_double() -> @type[[TYPE_nested_double]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: @type[[TYPE_nested_double]] [storage=automatic] = aggregate<@type[[TYPE_nested_double]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<array<f64, 1>, zero_fill=false>(index0 = const<f64>(2.5))));
// PCC-NEXT:         return copy<@type[[TYPE_nested_double]], reason=return>(read<@type[[TYPE_nested_double]]>(%[[VALUE_r_6]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_word:[0-9]+]] @ret_word() -> @type[[TYPE_word]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: @type[[TYPE_word]] [storage=automatic] = aggregate<@type[[TYPE_word]], zero_fill=false>(field0 = const<i32>(7));
// PCC-NEXT:         return copy<@type[[TYPE_word]], reason=return>(read<@type[[TYPE_word]]>(%[[VALUE_r_7]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// PCC-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// PCC-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// PCC-NEXT:         return read<i32>(%[[VALUE_a_3]]);
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// PCC-NEXT:         return add<i32>(read<i32>(%[[VALUE_argc]]), from_bool<i32>(ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<ptr<i8>>>)));
// PCC-NEXT:     }
// PCC-NEXT: }
// SLATE-FILECHECK-END PCC
// SLATE-FILECHECK-BEGIN RTD
// RTD: module {
// RTD-NEXT:     target "i686-unknown-linux-gnu" {
// RTD-NEXT:         endian = little;
// RTD-NEXT:         pointer [size=4, align=4];
// RTD-NEXT:         stack_alignment = 16;
// RTD-NEXT:         long_double = f80;
// RTD-NEXT:         storage bool [size=1, align=1];
// RTD-NEXT:         storage i8, u8 [size=1, align=1];
// RTD-NEXT:         storage i16, u16 [size=2, align=2];
// RTD-NEXT:         storage i32, u32 [size=4, align=4];
// RTD-NEXT:         storage i64, u64 [size=8, align=4];
// RTD-NEXT:         storage i128, u128 [size=16, align=16];
// RTD-NEXT:         storage bf16 [size=2, align=2];
// RTD-NEXT:         storage f16 [size=2, align=2];
// RTD-NEXT:         storage f32 [size=4, align=4];
// RTD-NEXT:         storage f64 [size=8, align=4];
// RTD-NEXT:         storage f80 [size=12, align=4];
// RTD-NEXT:         storage f128 [size=16, align=16];
// RTD-NEXT:         storage d32 [size=4, align=4];
// RTD-NEXT:         storage d64 [size=8, align=8];
// RTD-NEXT:         storage d128 [size=16, align=16];
// RTD-NEXT:     }
// RTD-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// RTD-NEXT:     } [size=0, align=1, offsets=[]];
// RTD-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// RTD-NEXT:         field0 a: array<i8, 3>;
// RTD-NEXT:     } [size=3, align=1, offsets=[0]];
// RTD-NEXT:     type @type[[TYPE_four:[0-9]+]] four = struct {
// RTD-NEXT:         field0 a: i16;
// RTD-NEXT:         field1 b: i8;
// RTD-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// RTD-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// RTD-NEXT:         field0 a: i32;
// RTD-NEXT:         field1 b: i32;
// RTD-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// RTD-NEXT:     type @type[[TYPE_single_float:[0-9]+]] single_float = struct {
// RTD-NEXT:         field0 f: f32;
// RTD-NEXT:     } [size=4, align=4, offsets=[0]];
// RTD-NEXT:     type @type[[TYPE_nested_double:[0-9]+]] nested_double = struct {
// RTD-NEXT:         field0 inner: @type[[TYPE0:[0-9]+]];
// RTD-NEXT:     } [size=8, align=4, offsets=[0]];
// RTD-NEXT:     type @type[[TYPE0]] = struct {
// RTD-NEXT:         field0 d: array<f64, 1>;
// RTD-NEXT:     } [size=8, align=4, offsets=[0]];
// RTD-NEXT:     type @type[[TYPE_word:[0-9]+]] word = union {
// RTD-NEXT:         field0 i: i32;
// RTD-NEXT:         field1 c: i8;
// RTD-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// RTD-NEXT:     fn %[[VALUE_ret_empty:[0-9]+]] @ret_empty() -> @type[[TYPE_empty]] [linkage=external] [abi=x86_cdecl stdcall() -> sret<align=1>] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_empty]] [storage=automatic];
// RTD-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_r]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_ret_three:[0-9]+]] @ret_three() -> @type[[TYPE_three]] [linkage=external] [abi=x86_cdecl stdcall() -> native_c] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_three]] [storage=automatic] = aggregate<@type[[TYPE_three]], zero_fill=false>(field0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8>(const<i32>(1)), index1 = truncate<i8>(const<i32>(2)), index2 = truncate<i8>(const<i32>(3))));
// RTD-NEXT:         return copy<@type[[TYPE_three]], reason=return>(read<@type[[TYPE_three]]>(%[[VALUE_r_2]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_ret_four:[0-9]+]] @ret_four() -> @type[[TYPE_four]] [linkage=external] [abi=x86_cdecl stdcall() -> native_c] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_four]] [storage=automatic] = aggregate<@type[[TYPE_four]], zero_fill=false>(field0 = truncate<i16>(const<i32>(1)), field1 = truncate<i8>(const<i32>(2)));
// RTD-NEXT:         return copy<@type[[TYPE_four]], reason=return>(read<@type[[TYPE_four]]>(%[[VALUE_r_3]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_ret_eight:[0-9]+]] @ret_eight() -> @type[[TYPE_eight]] [linkage=external] [abi=x86_cdecl stdcall() -> native_c] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: @type[[TYPE_eight]] [storage=automatic] = aggregate<@type[[TYPE_eight]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// RTD-NEXT:         return copy<@type[[TYPE_eight]], reason=return>(read<@type[[TYPE_eight]]>(%[[VALUE_r_4]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_ret_single_float:[0-9]+]] @ret_single_float() -> @type[[TYPE_single_float]] [linkage=external] [abi=x86_cdecl stdcall() -> native_c] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: @type[[TYPE_single_float]] [storage=automatic] = aggregate<@type[[TYPE_single_float]], zero_fill=false>(field0 = const<f32>(1.5));
// RTD-NEXT:         return copy<@type[[TYPE_single_float]], reason=return>(read<@type[[TYPE_single_float]]>(%[[VALUE_r_5]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_ret_nested_double:[0-9]+]] @ret_nested_double() -> @type[[TYPE_nested_double]] [linkage=external] [abi=x86_cdecl stdcall() -> native_c] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: @type[[TYPE_nested_double]] [storage=automatic] = aggregate<@type[[TYPE_nested_double]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<array<f64, 1>, zero_fill=false>(index0 = const<f64>(2.5))));
// RTD-NEXT:         return copy<@type[[TYPE_nested_double]], reason=return>(read<@type[[TYPE_nested_double]]>(%[[VALUE_r_6]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_ret_word:[0-9]+]] @ret_word() -> @type[[TYPE_word]] [linkage=external] [abi=x86_cdecl stdcall() -> native_c] [fallthrough=ub_if_used] {
// RTD-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: @type[[TYPE_word]] [storage=automatic] = aggregate<@type[[TYPE_word]], zero_fill=false>(field0 = const<i32>(7));
// RTD-NEXT:         return copy<@type[[TYPE_word]], reason=return>(read<@type[[TYPE_word]]>(%[[VALUE_r_7]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// RTD-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// RTD-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// RTD-NEXT:         return read<i32>(%[[VALUE_a_3]]);
// RTD-NEXT:     }
// RTD-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// RTD-NEXT:         return add<i32>(read<i32>(%[[VALUE_argc]]), from_bool<i32>(ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<ptr<i8>>>)));
// RTD-NEXT:     }
// RTD-NEXT: }
// SLATE-FILECHECK-END RTD
// SLATE-FILECHECK-BEGIN NORTD
// NORTD: module {
// NORTD-NEXT:     target "i686-unknown-linux-gnu" {
// NORTD-NEXT:         endian = little;
// NORTD-NEXT:         pointer [size=4, align=4];
// NORTD-NEXT:         stack_alignment = 16;
// NORTD-NEXT:         long_double = f80;
// NORTD-NEXT:         storage bool [size=1, align=1];
// NORTD-NEXT:         storage i8, u8 [size=1, align=1];
// NORTD-NEXT:         storage i16, u16 [size=2, align=2];
// NORTD-NEXT:         storage i32, u32 [size=4, align=4];
// NORTD-NEXT:         storage i64, u64 [size=8, align=4];
// NORTD-NEXT:         storage i128, u128 [size=16, align=16];
// NORTD-NEXT:         storage bf16 [size=2, align=2];
// NORTD-NEXT:         storage f16 [size=2, align=2];
// NORTD-NEXT:         storage f32 [size=4, align=4];
// NORTD-NEXT:         storage f64 [size=8, align=4];
// NORTD-NEXT:         storage f80 [size=12, align=4];
// NORTD-NEXT:         storage f128 [size=16, align=16];
// NORTD-NEXT:         storage d32 [size=4, align=4];
// NORTD-NEXT:         storage d64 [size=8, align=8];
// NORTD-NEXT:         storage d128 [size=16, align=16];
// NORTD-NEXT:     }
// NORTD-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// NORTD-NEXT:     } [size=0, align=1, offsets=[]];
// NORTD-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// NORTD-NEXT:         field0 a: array<i8, 3>;
// NORTD-NEXT:     } [size=3, align=1, offsets=[0]];
// NORTD-NEXT:     type @type[[TYPE_four:[0-9]+]] four = struct {
// NORTD-NEXT:         field0 a: i16;
// NORTD-NEXT:         field1 b: i8;
// NORTD-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// NORTD-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// NORTD-NEXT:         field0 a: i32;
// NORTD-NEXT:         field1 b: i32;
// NORTD-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// NORTD-NEXT:     type @type[[TYPE_single_float:[0-9]+]] single_float = struct {
// NORTD-NEXT:         field0 f: f32;
// NORTD-NEXT:     } [size=4, align=4, offsets=[0]];
// NORTD-NEXT:     type @type[[TYPE_nested_double:[0-9]+]] nested_double = struct {
// NORTD-NEXT:         field0 inner: @type[[TYPE0:[0-9]+]];
// NORTD-NEXT:     } [size=8, align=4, offsets=[0]];
// NORTD-NEXT:     type @type[[TYPE0]] = struct {
// NORTD-NEXT:         field0 d: array<f64, 1>;
// NORTD-NEXT:     } [size=8, align=4, offsets=[0]];
// NORTD-NEXT:     type @type[[TYPE_word:[0-9]+]] word = union {
// NORTD-NEXT:         field0 i: i32;
// NORTD-NEXT:         field1 c: i8;
// NORTD-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// NORTD-NEXT:     fn %[[VALUE_ret_empty:[0-9]+]] @ret_empty() -> @type[[TYPE_empty]] [linkage=external] [abi=x86_cdecl() -> sret<align=1>] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_empty]] [storage=automatic];
// NORTD-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_r]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_ret_three:[0-9]+]] @ret_three() -> @type[[TYPE_three]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_three]] [storage=automatic] = aggregate<@type[[TYPE_three]], zero_fill=false>(field0 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8>(const<i32>(1)), index1 = truncate<i8>(const<i32>(2)), index2 = truncate<i8>(const<i32>(3))));
// NORTD-NEXT:         return copy<@type[[TYPE_three]], reason=return>(read<@type[[TYPE_three]]>(%[[VALUE_r_2]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_ret_four:[0-9]+]] @ret_four() -> @type[[TYPE_four]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_four]] [storage=automatic] = aggregate<@type[[TYPE_four]], zero_fill=false>(field0 = truncate<i16>(const<i32>(1)), field1 = truncate<i8>(const<i32>(2)));
// NORTD-NEXT:         return copy<@type[[TYPE_four]], reason=return>(read<@type[[TYPE_four]]>(%[[VALUE_r_3]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_ret_eight:[0-9]+]] @ret_eight() -> @type[[TYPE_eight]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: @type[[TYPE_eight]] [storage=automatic] = aggregate<@type[[TYPE_eight]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// NORTD-NEXT:         return copy<@type[[TYPE_eight]], reason=return>(read<@type[[TYPE_eight]]>(%[[VALUE_r_4]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_ret_single_float:[0-9]+]] @ret_single_float() -> @type[[TYPE_single_float]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: @type[[TYPE_single_float]] [storage=automatic] = aggregate<@type[[TYPE_single_float]], zero_fill=false>(field0 = const<f32>(1.5));
// NORTD-NEXT:         return copy<@type[[TYPE_single_float]], reason=return>(read<@type[[TYPE_single_float]]>(%[[VALUE_r_5]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_ret_nested_double:[0-9]+]] @ret_nested_double() -> @type[[TYPE_nested_double]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: @type[[TYPE_nested_double]] [storage=automatic] = aggregate<@type[[TYPE_nested_double]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = aggregate<array<f64, 1>, zero_fill=false>(index0 = const<f64>(2.5))));
// NORTD-NEXT:         return copy<@type[[TYPE_nested_double]], reason=return>(read<@type[[TYPE_nested_double]]>(%[[VALUE_r_6]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_ret_word:[0-9]+]] @ret_word() -> @type[[TYPE_word]] [linkage=external] [abi=x86_cdecl() -> native_c] [fallthrough=ub_if_used] {
// NORTD-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: @type[[TYPE_word]] [storage=automatic] = aggregate<@type[[TYPE_word]], zero_fill=false>(field0 = const<i32>(7));
// NORTD-NEXT:         return copy<@type[[TYPE_word]], reason=return>(read<@type[[TYPE_word]]>(%[[VALUE_r_7]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// NORTD-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// NORTD-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_explicit_cdecl:[0-9]+]] @explicit_cdecl(%[[VALUE_a_3:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// NORTD-NEXT:         return read<i32>(%[[VALUE_a_3]]);
// NORTD-NEXT:     }
// NORTD-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// NORTD-NEXT:         return add<i32>(read<i32>(%[[VALUE_argc]]), from_bool<i32>(ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), null<ptr<ptr<i8>>>)));
// NORTD-NEXT:     }
// NORTD-NEXT: }
// SLATE-FILECHECK-END NORTD
