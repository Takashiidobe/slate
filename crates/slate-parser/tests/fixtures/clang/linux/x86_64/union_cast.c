typedef int T;
struct P { int x; };
union U { int : 3; int i; float f; int j; };
union V { const T t; };
union A { char *p; void (*fp)(void); };
union S { struct P p; };
union B { int b : 3; };
void h(void);
char g[4];

union U cast_int(int x) { return (union U)x; }
union U cast_float(float x) { return (union U)x; }
union V cast_const(const int x) { return (union V)x; }
union A cast_array(void) { return (union A)g; }
union A cast_function(void) { return (union A)h; }
union S cast_record(struct P p) { return (union S)p; }
union U cast_same(union U u) { return (union U)u; }
union B cast_bit(int x) { return (union B)x; }
const union U cast_qualified(int x) { return (const union U)x; }
union U global = (union U)1;
int read_member(int x) { return ((union U)x).i; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES IR

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
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
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 <anonymous>: i32 : 3;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:         field2 f: f32;
// IR-NEXT:         field3 j: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 0], bit_offsets=[Some(0), None, None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None, None]];
// IR-NEXT:     type @type[[TYPE_V:[0-9]+]] V = union {
// IR-NEXT:         field0 t: const i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_A:[0-9]+]] A = union {
// IR-NEXT:         field0 p: ptr<i8>;
// IR-NEXT:         field1 fp: ptr<fn() -> void>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = union {
// IR-NEXT:         field0 p: @type[[TYPE_P]];
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_B:[0-9]+]] B = union {
// IR-NEXT:         field0 b: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: array<i8, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_global:[0-9]+]] global: @type[[TYPE_U]] [storage=static] = copy<@type[[TYPE_U]], reason=assign>(aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = const<i32>(1))) [linkage=external];
// IR-NEXT:     fn %[[VALUE_h:[0-9]+]] @h() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_cast_int:[0-9]+]] @cast_int(%[[VALUE_x:[0-9]+]] x: i32) -> @type[[TYPE_U]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_U]], reason=return>(aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = read<i32>(%[[VALUE_x]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_float:[0-9]+]] @cast_float(%[[VALUE_x_2:[0-9]+]] x: f32) -> @type[[TYPE_U]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_U]], reason=return>(aggregate<@type[[TYPE_U]], zero_fill=false>(field2 = read<f32>(%[[VALUE_x_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_const:[0-9]+]] @cast_const(%[[VALUE_x_3:[0-9]+]] x: i32 [const]) -> @type[[TYPE_V]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_V]], reason=return>(aggregate<@type[[TYPE_V]], zero_fill=false>(field0 = read<i32>(%[[VALUE_x_3]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_array:[0-9]+]] @cast_array() -> @type[[TYPE_A]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_A]], reason=return>(aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_g]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_function:[0-9]+]] @cast_function() -> @type[[TYPE_A]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_A]], reason=return>(aggregate<@type[[TYPE_A]], zero_fill=false>(field1 = function_decay<ptr<fn() -> void>>(%[[VALUE_h]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_record:[0-9]+]] @cast_record(%[[VALUE_p:[0-9]+]] p: @type[[TYPE_P]]) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_S]], reason=return>(aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = read<@type[[TYPE_P]]>(%[[VALUE_p]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_same:[0-9]+]] @cast_same(%[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]]) -> @type[[TYPE_U]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_U]], reason=return>(read<@type[[TYPE_U]]>(%[[VALUE_u]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_bit:[0-9]+]] @cast_bit(%[[VALUE_x_4:[0-9]+]] x: i32) -> @type[[TYPE_B]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_B]], reason=return>(aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = read<i32>(%[[VALUE_x_4]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast_qualified:[0-9]+]] @cast_qualified(%[[VALUE_x_5:[0-9]+]] x: i32) -> @type[[TYPE_U]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_U]], reason=return>(aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = read<i32>(%[[VALUE_x_5]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_read_member:[0-9]+]] @read_member(%[[VALUE_x_6:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(field1(temporary %[[VALUE0:[0-9]+]] = aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = read<i32>(%[[VALUE_x_6]]))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
