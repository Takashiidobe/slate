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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Tag = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %1 table: array<i32, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %29 .str29: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// IR-NEXT:     fn %2 @routine() -> void [linkage=external];
// IR-NEXT:     fn %3 @constant_branch(%4 i: i32, %5 d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @runtime_branch(%7 i: i32, %8 l: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @string_branch(%10 d: f64) -> ptr<const i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<const i8>, reason=return>(array_decay<ptr<i8>, length=Some(7)>(%29));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @type_operand() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %12 @array_decays() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %13 @function_decays() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %14 @qualifier_stripped(%15 c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %16 @record_tag(%17 t: @type0) -> i32 [linkage=external] [abi=sysv64(coerce<i64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(field0(%17));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @nested(%19 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %20 @as_condition(%21 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<i32>(read<i32>(%21), const<i32>(0))
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %22 @as_place(%23 x: i32, %24 y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(%23, const<i32>(5));
// IR-NEXT:         return read<i32>(%23);
// IR-NEXT:     }
// IR-NEXT:     fn %25 @unevaluated_controlling() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %26 @side_effect_branch(%27 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %28 n: i32 [storage=automatic];
// IR-NEXT:         let %30: i32 [synthetic] = read<i32>(%27);
// IR-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// IR-NEXT:         write<i32>(%27, read<i32>(%31));
// IR-NEXT:         write<i32>(%28, read<i32>(%30));
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%28), read<i32>(%27));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
