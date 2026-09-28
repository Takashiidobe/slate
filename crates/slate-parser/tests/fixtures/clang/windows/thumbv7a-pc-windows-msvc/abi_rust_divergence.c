// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct floats { float a, b; };
struct doubles { double a, b; };
struct mixed { float a; int b; };
struct five { float a, b, c, d, e; };

struct floats floats(struct floats value) { return value; }
struct doubles doubles(struct doubles value) { return value; }
struct mixed mixed(struct mixed value) { return value; }
struct five five(struct five value) { return value; }
_Complex float complex_float(_Complex float value) { return value; }
_Complex int complex_int(_Complex int value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "thumbv7a-pc-windows-msvc" {
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
// IR-NEXT:     type @type1 doubles = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type2 mixed = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type3 five = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:         field2 c: f32;
// IR-NEXT:         field3 d: f32;
// IR-NEXT:         field4 e: f32;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// IR-NEXT:     fn %4 @floats(%5 value: @type0) -> @type0 [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%5));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @doubles(%7 value: @type1) -> @type1 [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%7));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @mixed(%9 value: @type2) -> @type2 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @five(%11 value: @type3) -> @type3 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @complex_float(%13 value: complex<f32>) -> complex<f32> [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%13);
// IR-NEXT:     }
// IR-NEXT:     fn %14 @complex_int(%15 value: complex<i32>) -> complex<i32> [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<i32>>(%15);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
