struct empty {};
struct eight { int a, b; };
union word { int i; char c; };
_Complex float ret_complex(void) { return 1.0f; }
struct empty ret_empty(void) { struct empty r; return r; }
struct eight ret_eight(void) { struct eight r = {1, 2}; return r; }
union word ret_word(void) { union word r = {7}; return r; }
int plain(int a, int b) { return a + b; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES PCC
// SLATE-FILECHECK-PREFIX-ARGS PCC -fpcc-struct-return
// SLATE-FILECHECK-DEFINES IGNORED
// SLATE-FILECHECK-PREFIX-ARGS IGNORED -freg-struct-return -mrtd

// SLATE-FILECHECK-BEGIN PCC
// PCC: module {
// PCC-NEXT:     target "x86_64-unknown-linux-gnu" {
// PCC-NEXT:         endian = little;
// PCC-NEXT:         pointer [size=8, align=8];
// PCC-NEXT:         stack_alignment = 16;
// PCC-NEXT:         long_double = f80;
// PCC-NEXT:         storage bool [size=1, align=1];
// PCC-NEXT:         storage i8, u8 [size=1, align=1];
// PCC-NEXT:         storage i16, u16 [size=2, align=2];
// PCC-NEXT:         storage i32, u32 [size=4, align=4];
// PCC-NEXT:         storage i64, u64 [size=8, align=8];
// PCC-NEXT:         storage i128, u128 [size=16, align=16];
// PCC-NEXT:         storage bf16 [size=2, align=2];
// PCC-NEXT:         storage f16 [size=2, align=2];
// PCC-NEXT:         storage f32 [size=4, align=4];
// PCC-NEXT:         storage f64 [size=8, align=8];
// PCC-NEXT:         storage f80 [size=16, align=16];
// PCC-NEXT:         storage f128 [size=16, align=16];
// PCC-NEXT:         storage d32 [size=4, align=4];
// PCC-NEXT:         storage d64 [size=8, align=8];
// PCC-NEXT:         storage d128 [size=16, align=16];
// PCC-NEXT:     }
// PCC-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// PCC-NEXT:     } [size=0, align=1, offsets=[]];
// PCC-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// PCC-NEXT:         field0 a: i32;
// PCC-NEXT:         field1 b: i32;
// PCC-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// PCC-NEXT:     type @type[[TYPE_word:[0-9]+]] word = union {
// PCC-NEXT:         field0 i: i32;
// PCC-NEXT:         field1 c: i8;
// PCC-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// PCC-NEXT:     fn %[[VALUE_ret_complex:[0-9]+]] @ret_complex() -> complex<f32> [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         return real_to_complex<complex<f32>>(const<f32>(1.0));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_empty:[0-9]+]] @ret_empty() -> @type[[TYPE_empty]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_empty]] [storage=automatic];
// PCC-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_r]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_eight:[0-9]+]] @ret_eight() -> @type[[TYPE_eight]] [linkage=external] [abi=sysv64() -> sret<align=4>] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_eight]] [storage=automatic] = aggregate<@type[[TYPE_eight]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// PCC-NEXT:         return copy<@type[[TYPE_eight]], reason=return>(read<@type[[TYPE_eight]]>(%[[VALUE_r_2]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_ret_word:[0-9]+]] @ret_word() -> @type[[TYPE_word]] [linkage=external] [abi=sysv64() -> sret<align=4>] [fallthrough=ub_if_used] {
// PCC-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_word]] [storage=automatic] = aggregate<@type[[TYPE_word]], zero_fill=false>(field0 = const<i32>(7));
// PCC-NEXT:         return copy<@type[[TYPE_word]], reason=return>(read<@type[[TYPE_word]]>(%[[VALUE_r_3]]));
// PCC-NEXT:     }
// PCC-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// PCC-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// PCC-NEXT:     }
// PCC-NEXT: }
// SLATE-FILECHECK-END PCC
// SLATE-FILECHECK-BEGIN IGNORED
// IGNORED: module {
// IGNORED-NEXT:     target "x86_64-unknown-linux-gnu" {
// IGNORED-NEXT:         endian = little;
// IGNORED-NEXT:         pointer [size=8, align=8];
// IGNORED-NEXT:         stack_alignment = 16;
// IGNORED-NEXT:         long_double = f80;
// IGNORED-NEXT:         storage bool [size=1, align=1];
// IGNORED-NEXT:         storage i8, u8 [size=1, align=1];
// IGNORED-NEXT:         storage i16, u16 [size=2, align=2];
// IGNORED-NEXT:         storage i32, u32 [size=4, align=4];
// IGNORED-NEXT:         storage i64, u64 [size=8, align=8];
// IGNORED-NEXT:         storage i128, u128 [size=16, align=16];
// IGNORED-NEXT:         storage bf16 [size=2, align=2];
// IGNORED-NEXT:         storage f16 [size=2, align=2];
// IGNORED-NEXT:         storage f32 [size=4, align=4];
// IGNORED-NEXT:         storage f64 [size=8, align=8];
// IGNORED-NEXT:         storage f80 [size=16, align=16];
// IGNORED-NEXT:         storage f128 [size=16, align=16];
// IGNORED-NEXT:         storage d32 [size=4, align=4];
// IGNORED-NEXT:         storage d64 [size=8, align=8];
// IGNORED-NEXT:         storage d128 [size=16, align=16];
// IGNORED-NEXT:     }
// IGNORED-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// IGNORED-NEXT:     } [size=0, align=1, offsets=[]];
// IGNORED-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// IGNORED-NEXT:         field0 a: i32;
// IGNORED-NEXT:         field1 b: i32;
// IGNORED-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IGNORED-NEXT:     type @type[[TYPE_word:[0-9]+]] word = union {
// IGNORED-NEXT:         field0 i: i32;
// IGNORED-NEXT:         field1 c: i8;
// IGNORED-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IGNORED-NEXT:     fn %[[VALUE_ret_complex:[0-9]+]] @ret_complex() -> complex<f32> [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IGNORED-NEXT:         return real_to_complex<complex<f32>>(const<f32>(1.0));
// IGNORED-NEXT:     }
// IGNORED-NEXT:     fn %[[VALUE_ret_empty:[0-9]+]] @ret_empty() -> @type[[TYPE_empty]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IGNORED-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_empty]] [storage=automatic];
// IGNORED-NEXT:         return copy<@type[[TYPE_empty]], reason=return>(read<@type[[TYPE_empty]]>(%[[VALUE_r]]));
// IGNORED-NEXT:     }
// IGNORED-NEXT:     fn %[[VALUE_ret_eight:[0-9]+]] @ret_eight() -> @type[[TYPE_eight]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IGNORED-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_eight]] [storage=automatic] = aggregate<@type[[TYPE_eight]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2));
// IGNORED-NEXT:         return copy<@type[[TYPE_eight]], reason=return>(read<@type[[TYPE_eight]]>(%[[VALUE_r_2]]));
// IGNORED-NEXT:     }
// IGNORED-NEXT:     fn %[[VALUE_ret_word:[0-9]+]] @ret_word() -> @type[[TYPE_word]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IGNORED-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_word]] [storage=automatic] = aggregate<@type[[TYPE_word]], zero_fill=false>(field0 = const<i32>(7));
// IGNORED-NEXT:         return copy<@type[[TYPE_word]], reason=return>(read<@type[[TYPE_word]]>(%[[VALUE_r_3]]));
// IGNORED-NEXT:     }
// IGNORED-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IGNORED-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// IGNORED-NEXT:     }
// IGNORED-NEXT: }
// SLATE-FILECHECK-END IGNORED
