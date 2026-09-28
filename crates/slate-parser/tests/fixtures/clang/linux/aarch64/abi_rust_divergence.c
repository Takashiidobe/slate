// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct floats { float a, b; };
struct zero_tail { double d; float tail[0]; };
struct flexible { double d; float tail[]; };
struct empty {};
struct empty_then_float { struct empty e; float f; };
struct halves { _Float16 a, b; };
struct long_double { long double x; };

struct floats floats(struct floats value) { return value; }
struct zero_tail zero_tail(struct zero_tail value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct empty_then_float empty_then_float(struct empty_then_float value) { return value; }
struct halves halves(struct halves value) { return value; }
struct long_double long_double(struct long_double value) { return value; }
_Complex _Float16 complex_half(_Complex _Float16 value) { return value; }
_Complex long double complex_long_double(_Complex long double value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:     type @type0 floats = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 zero_tail = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 tail: array<f32, 0>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type2 flexible = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 tail: array<f32, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type3 empty = struct {
// IR-NEXT:     } [size=0, align=1, offsets=[]];
// IR-NEXT:     type @type4 empty_then_float = struct {
// IR-NEXT:         field0 e: @type3;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type5 halves = struct {
// IR-NEXT:         field0 a: f16;
// IR-NEXT:         field1 b: f16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type6 long_double = struct {
// IR-NEXT:         field0 x: f128;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     fn %7 @floats(%8 value: @type0) -> @type0 [linkage=external] [abi=aapcs64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%8));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @zero_tail(%10 value: @type1) -> @type1 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @flexible(%12 value: @type2) -> @type2 [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @empty_then_float(%14 value: @type4) -> @type4 [linkage=external] [abi=aapcs64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(read<@type4>(%14));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @halves(%16 value: @type5) -> @type5 [linkage=external] [abi=aapcs64(coerce<f16, f16>) -> coerce<f16, f16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type5, reason=return>(read<@type5>(%16));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @long_double(%18 value: @type6) -> @type6 [linkage=external] [abi=aapcs64(coerce<f128>) -> coerce<f128>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type6, reason=return>(read<@type6>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @complex_half(%20 value: complex<f16>) -> complex<f16> [linkage=external] [abi=aapcs64(coerce<f16, f16>) -> coerce<f16, f16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f16>>(%20);
// IR-NEXT:     }
// IR-NEXT:     fn %21 @complex_long_double(%22 value: complex<f128>) -> complex<f128> [linkage=external] [abi=aapcs64(coerce<f128, f128>) -> coerce<f128, f128>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f128>>(%22);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
