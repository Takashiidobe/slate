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
// IR-NEXT:     type @type0 T = i32;
// IR-NEXT:     type @type1 P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type2 U = union {
// IR-NEXT:         field0 <anonymous>: i32 : 3;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:         field2 f: f32;
// IR-NEXT:         field3 j: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 0], bit_offsets=[Some(0), None, None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None, None]];
// IR-NEXT:     type @type3 V = union {
// IR-NEXT:         field0 t: const i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type4 A = union {
// IR-NEXT:         field0 p: ptr<i8>;
// IR-NEXT:         field1 fp: ptr<fn() -> void>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type5 S = union {
// IR-NEXT:         field0 p: @type1;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type6 B = union {
// IR-NEXT:         field0 b: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     global %8 g: array<i8, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %25 global: @type2 [storage=static] = copy<@type2, reason=assign>(aggregate<@type2, zero_fill=false>(field1 = const<i32>(1))) [linkage=external];
// IR-NEXT:     fn %7 @h() -> void [linkage=external];
// IR-NEXT:     fn %9 @cast_int(%10 x: i32) -> @type2 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(aggregate<@type2, zero_fill=false>(field1 = read<i32>(%10)));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @cast_float(%12 x: f32) -> @type2 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(aggregate<@type2, zero_fill=false>(field2 = read<f32>(%12)));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @cast_const(%14 x: i32 [const]) -> @type3 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(aggregate<@type3, zero_fill=false>(field0 = read<i32>(%14)));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @cast_array() -> @type4 [linkage=external] [abi=sysv64() -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(aggregate<@type4, zero_fill=false>(field0 = array_decay<ptr<i8>, length=Some(4)>(%8)));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @cast_function() -> @type4 [linkage=external] [abi=sysv64() -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(aggregate<@type4, zero_fill=false>(field1 = function_decay<ptr<fn() -> void>>(%7)));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @cast_record(%18 p: @type1) -> @type5 [linkage=external] [abi=sysv64(coerce<i32>) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type5, reason=return>(aggregate<@type5, zero_fill=false>(field0 = read<@type1>(%18)));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @cast_same(%20 u: @type2) -> @type2 [linkage=external] [abi=sysv64(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%20));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @cast_bit(%22 x: i32) -> @type6 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type6, reason=return>(aggregate<@type6, zero_fill=false>(field0 = read<i32>(%22)));
// IR-NEXT:     }
// IR-NEXT:     fn %23 @cast_qualified(%24 x: i32) -> @type2 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(aggregate<@type2, zero_fill=false>(field1 = read<i32>(%24)));
// IR-NEXT:     }
// IR-NEXT:     fn %26 @read_member(%27 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(field1(temporary %28 = aggregate<@type2, zero_fill=false>(field1 = read<i32>(%27))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
