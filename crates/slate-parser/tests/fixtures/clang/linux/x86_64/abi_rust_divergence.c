// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct plain { int a; float b; };
struct flexible { int n; int tail[]; };
struct zero_tail { float f; int tail[0]; };
struct halves { _Float16 a, b; };
struct half_float { _Float16 a; float b; };
struct double_half { double d; _Float16 h; };
struct brain_floats { __bf16 a, b; };
struct long_double { long double x; };
struct quad { __float128 q; };
union half_or_float { _Float16 h; float f; };
struct half_int { _Float16 h; int i; };

struct plain plain(struct plain value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct zero_tail zero_tail(struct zero_tail value) { return value; }
struct halves halves(struct halves value) { return value; }
struct half_float half_float(struct half_float value) { return value; }
struct double_half double_half(struct double_half value) { return value; }
struct brain_floats brain_floats(struct brain_floats value) { return value; }
struct long_double long_double(struct long_double value) { return value; }
struct quad quad(struct quad value) { return value; }
union half_or_float half_or_float(union half_or_float value) { return value; }
struct half_int half_int(struct half_int value) { return value; }
_Complex _Float16 complex_half(_Complex _Float16 value) { return value; }
_Complex __float128 complex_quad(_Complex __float128 value) { return value; }
_Complex long double complex_long_double(_Complex long double value) { return value; }
_Complex float complex_float(_Complex float value) { return value; }

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
// IR-NEXT:     type @type0 plain = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 flexible = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 tail: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 zero_tail = struct {
// IR-NEXT:         field0 f: f32;
// IR-NEXT:         field1 tail: array<i32, 0>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type3 halves = struct {
// IR-NEXT:         field0 a: f16;
// IR-NEXT:         field1 b: f16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type4 half_float = struct {
// IR-NEXT:         field0 a: f16;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type5 double_half = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 h: f16;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type6 brain_floats = struct {
// IR-NEXT:         field0 a: bf16;
// IR-NEXT:         field1 b: bf16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type7 long_double = struct {
// IR-NEXT:         field0 x: f80;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     type @type8 quad = struct {
// IR-NEXT:         field0 q: f128;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     type @type9 half_or_float = union {
// IR-NEXT:         field0 h: f16;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type10 half_int = struct {
// IR-NEXT:         field0 h: f16;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %11 @plain(%12 value: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @flexible(%14 value: @type1) -> @type1 [linkage=external] [abi=sysv64(byval<align=4>) -> sret<align=4>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%14));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @zero_tail(%16 value: @type2) -> @type2 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%16));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @halves(%18 value: @type3) -> @type3 [linkage=external] [abi=sysv64(coerce<pair<f16>>) -> coerce<pair<f16>>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @half_float(%20 value: @type4) -> @type4 [linkage=external] [abi=sysv64(coerce<quad<f16>>) -> coerce<quad<f16>>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(read<@type4>(%20));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @double_half(%22 value: @type5) -> @type5 [linkage=external] [abi=sysv64(coerce<f64, f16>) -> coerce<f64, f16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type5, reason=return>(read<@type5>(%22));
// IR-NEXT:     }
// IR-NEXT:     fn %23 @brain_floats(%24 value: @type6) -> @type6 [linkage=external] [abi=sysv64(coerce<pair<bf16>>) -> coerce<pair<bf16>>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type6, reason=return>(read<@type6>(%24));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @long_double(%26 value: @type7) -> @type7 [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type7, reason=return>(read<@type7>(%26));
// IR-NEXT:     }
// IR-NEXT:     fn %27 @quad(%28 value: @type8) -> @type8 [linkage=external] [abi=sysv64(coerce<f128>) -> coerce<f128>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type8, reason=return>(read<@type8>(%28));
// IR-NEXT:     }
// IR-NEXT:     fn %29 @half_or_float(%30 value: @type9) -> @type9 [linkage=external] [abi=sysv64(coerce<f32>) -> coerce<f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type9, reason=return>(read<@type9>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %31 @half_int(%32 value: @type10) -> @type10 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type10, reason=return>(read<@type10>(%32));
// IR-NEXT:     }
// IR-NEXT:     fn %33 @complex_half(%34 value: complex<f16>) -> complex<f16> [linkage=external] [abi=sysv64(coerce<pair<f16>>) -> coerce<pair<f16>>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f16>>(%34);
// IR-NEXT:     }
// IR-NEXT:     fn %35 @complex_quad(%36 value: complex<f128>) -> complex<f128> [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f128>>(%36);
// IR-NEXT:     }
// IR-NEXT:     fn %37 @complex_long_double(%38 value: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f80>>(%38);
// IR-NEXT:     }
// IR-NEXT:     fn %39 @complex_float(%40 value: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%40);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
