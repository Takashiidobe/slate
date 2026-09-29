// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct Tag { int a; };
int table[4];
void routine(void);

int constant_branch(int i, double d) { return _Generic(i, int: 1, double: 2, default: 3); }
int runtime_branch(int i, long l) { return _Generic(i, int: i + 1, default: l); }
const char *string_branch(double d) { return _Generic(d, int: "int", double: "double", default: "other"); }
int type_operand(void) { return _Generic(int, int: 1, default: 0); }
int array_decays(void) { return _Generic(table, int *: 1, default: 0); }
int function_decays(void) { return _Generic(routine, void (*)(void): 1, default: 0); }
int qualifier_stripped(const int c) { return _Generic(c, int: 1, default: 0); }
int record_tag(struct Tag t) { return _Generic(t, struct Tag: t.a, default: 0); }
int nested(int i) { return _Generic(i, int: _Generic(1.0f, float: 1, default: 0), default: 0); }
int as_condition(int i) { if (_Generic(i, int: i, default: 0)) return 1; return 0; }
int as_place(int x, int y) { _Generic(x, int: x, default: y) = 5; return x; }
int unevaluated_controlling(void) { return _Generic("abc", char *: 1, default: 0); }
int side_effect_branch(int i) { int n = _Generic(i, int: i++, default: 0); return n + i; }

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
// IR-NEXT:     type @type[[TYPE_Tag:[0-9]+]] Tag = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<i32, 4> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_routine:[0-9]+]] @routine() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_constant_branch:[0-9]+]] @constant_branch(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_d:[0-9]+]] d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_runtime_branch:[0-9]+]] @runtime_branch(%[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_l:[0-9]+]] l: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_string_branch:[0-9]+]] @string_branch(%[[VALUE_d_2:[0-9]+]] d: f64) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_type_operand:[0-9]+]] @type_operand() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_array_decays:[0-9]+]] @array_decays() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_function_decays:[0-9]+]] @function_decays() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_qualifier_stripped:[0-9]+]] @qualifier_stripped(%[[VALUE_c:[0-9]+]] c: i32 [const]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_tag:[0-9]+]] @record_tag(%[[VALUE_t:[0-9]+]] t: @type[[TYPE_Tag]]) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(field0(%[[VALUE_t]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_as_condition:[0-9]+]] @as_condition(%[[VALUE_i_4:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(0))
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_as_place:[0-9]+]] @as_place(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(5));
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unevaluated_controlling:[0-9]+]] @unevaluated_controlling() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_side_effect_branch:[0-9]+]] @side_effect_branch(%[[VALUE_i_5:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE0]]));
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), read<i32>(%[[VALUE_i_5]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
