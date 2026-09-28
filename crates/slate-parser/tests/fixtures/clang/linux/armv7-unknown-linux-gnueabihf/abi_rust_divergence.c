// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct floats { float a, b; };
struct zero_tail { float f; int tail[0]; };
struct flexible { double d; float tail[]; };
struct halves { _Float16 a, b; };

struct floats floats(struct floats value) { return value; }
struct zero_tail zero_tail(struct zero_tail value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct halves halves(struct halves value) { return value; }

void sink(int, ...);
void variadic_floats(struct floats value) { sink(1, value); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 floats = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 zero_tail = struct {
// IR-NEXT:         field0 f: f32;
// IR-NEXT:         field1 tail: array<i32, 0>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 flexible = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 tail: array<f32, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type3 halves = struct {
// IR-NEXT:         field0 a: f16;
// IR-NEXT:         field1 b: f16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     fn %4 @floats(%5 value: @type0) -> @type0 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%5));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @zero_tail(%7 value: @type1) -> @type1 [linkage=external] [abi=aapcs32_hard_float(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%7));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @flexible(%9 value: @type2) -> @type2 [linkage=external] [abi=aapcs32_hard_float(coerce<i64>) -> sret<align=8>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @halves(%11 value: @type3) -> @type3 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @sink(%15 <unnamed>: i32, ...) -> void [linkage=external];
// IR-NEXT:     fn %13 @variadic_floats(%14 value: @type0) -> void [linkage=external] [abi=aapcs32_hard_float(native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=aapcs32(scalar, native_c) -> void>(%12, const<i32>(1), copy<@type0, reason=vararg>(read<@type0>(%14)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
