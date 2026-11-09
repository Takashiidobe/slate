// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef struct { int a; } i1;
typedef struct { int a, b; } i2;
typedef struct { char a[3]; } c3;
typedef struct { long long a, b; } l2;
typedef struct { double a, b; } d2;
typedef struct { double a, b, c, d; } d4;
typedef struct { float a, b; } f2;
typedef struct { float a[5]; } f5;
typedef float v4f __attribute__((vector_size(16)));
typedef struct { v4f a, b; } hv2;
typedef struct __attribute__((aligned(8))) { int a, b; } al8;
i1 __vectorcall fi1(i1 x, int y);
i2 __vectorcall fi2(i2 x, int y);
c3 __vectorcall fc3(c3 x, int y);
l2 __vectorcall fl2(l2 x, int y);
d2 __vectorcall fd2(d2 x, int y);
d4 __vectorcall fd4(d4 x, int y);
f2 __vectorcall ff2(f2 x, int y);
f5 __vectorcall ff5(f5 x, int y);
hv2 __vectorcall fhv2(hv2 x, int y);
al8 __vectorcall fal8(al8 x, int y);
_Complex double __vectorcall fcd(_Complex double x, _Complex float y);
double __vectorcall floats(float a, double b, float c, double d, float e, double f, float g, double h);
v4f __vectorcall vectors(v4f a, v4f b, v4f c, v4f d, v4f e, v4f f, v4f g);
void __vectorcall late(d4 x, d2 y, double a, double b, double c);
void __vectorcall exhaust(d4 x, d2 y, double a, f2 z);
void __vectorcall many(double a, double b, double c, double d, double e, double f, double g, d2 x);
void __vectorcall atomic(_Atomic double a, d2 x);
int __vectorcall ints(long long a, int b, char c, _Bool d, void *e);
d2 use(d2 x, f2 y) { exhaust((d4){0}, x, 1, y); return fd2(x, 1); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 4;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_i1:[0-9]+]] i1 = @type[[TYPE0]];
// IR-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_i2:[0-9]+]] i2 = @type[[TYPE1]];
// IR-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// IR-NEXT:         field0 a: array<i8, 3>;
// IR-NEXT:     } [size=3, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_c3:[0-9]+]] c3 = @type[[TYPE2]];
// IR-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_l2:[0-9]+]] l2 = @type[[TYPE3]];
// IR-NEXT:     type @type[[TYPE4:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_d2:[0-9]+]] d2 = @type[[TYPE4]];
// IR-NEXT:     type @type[[TYPE5:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:         field2 c: f64;
// IR-NEXT:         field3 d: f64;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// IR-NEXT:     type @type[[TYPE_d4:[0-9]+]] d4 = @type[[TYPE5]];
// IR-NEXT:     type @type[[TYPE6:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_f2:[0-9]+]] f2 = @type[[TYPE6]];
// IR-NEXT:     type @type[[TYPE7:[0-9]+]] = struct {
// IR-NEXT:         field0 a: array<f32, 5>;
// IR-NEXT:     } [size=20, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_f5:[0-9]+]] f5 = @type[[TYPE7]];
// IR-NEXT:     type @type[[TYPE_v4f:[0-9]+]] v4f = vector<f32, 4>;
// IR-NEXT:     type @type[[TYPE8:[0-9]+]] = struct {
// IR-NEXT:         field0 a: vector<f32, 4> [c="v4f"];
// IR-NEXT:         field1 b: vector<f32, 4> [c="v4f"];
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type[[TYPE_hv2:[0-9]+]] hv2 = @type[[TYPE8]];
// IR-NEXT:     type @type[[TYPE9:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_al8:[0-9]+]] al8 = @type[[TYPE9]];
// IR-NEXT:     fn %[[VALUE_fi1:[0-9]+]] @fi1(%[[VALUE_x:[0-9]+]] x: @type[[TYPE0]] [c="i1"], %[[VALUE_y:[0-9]+]] y: i32) -> @type[[TYPE0]] [linkage=external] [abi=x86_win32 vectorcall(native_c, scalar) -> native_c] [c="i1(i1, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fi2:[0-9]+]] @fi2(%[[VALUE_x_2:[0-9]+]] x: @type[[TYPE1]] [c="i2"], %[[VALUE_y_2:[0-9]+]] y: i32) -> @type[[TYPE1]] [linkage=external] [abi=x86_win32 vectorcall(native_c, scalar) -> native_c] [c="i2(i2, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fc3:[0-9]+]] @fc3(%[[VALUE_x_3:[0-9]+]] x: @type[[TYPE2]] [c="c3"], %[[VALUE_y_3:[0-9]+]] y: i32) -> @type[[TYPE2]] [linkage=external] [abi=x86_win32 vectorcall(native_c, scalar) -> native_c] [c="c3(c3, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fl2:[0-9]+]] @fl2(%[[VALUE_x_4:[0-9]+]] x: @type[[TYPE3]] [c="l2"], %[[VALUE_y_4:[0-9]+]] y: i32) -> @type[[TYPE3]] [linkage=external] [abi=x86_win32 vectorcall(native_c, scalar) -> native_c] [c="l2(l2, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fd2:[0-9]+]] @fd2(%[[VALUE_x_5:[0-9]+]] x: @type[[TYPE4]] [c="d2"], %[[VALUE_y_5:[0-9]+]] y: i32) -> @type[[TYPE4]] [linkage=external] [abi=x86_win32 vectorcall(coerce<f64, f64>, scalar) -> coerce<f64, f64>] [c="d2(d2, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fd4:[0-9]+]] @fd4(%[[VALUE_x_6:[0-9]+]] x: @type[[TYPE5]] [c="d4"], %[[VALUE_y_6:[0-9]+]] y: i32) -> @type[[TYPE5]] [linkage=external] [abi=x86_win32 vectorcall(coerce<f64, f64, f64, f64>, scalar) -> coerce<f64, f64, f64, f64>] [c="d4(d4, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_ff2:[0-9]+]] @ff2(%[[VALUE_x_7:[0-9]+]] x: @type[[TYPE6]] [c="f2"], %[[VALUE_y_7:[0-9]+]] y: i32) -> @type[[TYPE6]] [linkage=external] [abi=x86_win32 vectorcall(coerce<f32, f32>, scalar) -> coerce<f32, f32>] [c="f2(f2, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_ff5:[0-9]+]] @ff5(%[[VALUE_x_8:[0-9]+]] x: @type[[TYPE7]] [c="f5"], %[[VALUE_y_8:[0-9]+]] y: i32) -> @type[[TYPE7]] [linkage=external] [abi=x86_win32 vectorcall(native_c, scalar) -> native_c] [c="f5(f5, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fhv2:[0-9]+]] @fhv2(%[[VALUE_x_9:[0-9]+]] x: @type[[TYPE8]] [c="hv2"], %[[VALUE_y_9:[0-9]+]] y: i32) -> @type[[TYPE8]] [linkage=external] [abi=x86_win32 vectorcall(coerce<vector<f32, 4>, vector<f32, 4>>, scalar) -> coerce<vector<f32, 4>, vector<f32, 4>>] [c="hv2(hv2, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fal8:[0-9]+]] @fal8(%[[VALUE_x_10:[0-9]+]] x: @type[[TYPE9]] [c="al8"], %[[VALUE_y_10:[0-9]+]] y: i32) -> @type[[TYPE9]] [linkage=external] [abi=x86_win32 vectorcall(byref<align=8>, scalar) -> native_c] [c="al8(al8, int) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_fcd:[0-9]+]] @fcd(%[[VALUE_x_11:[0-9]+]] x: complex<f64>, %[[VALUE_y_11:[0-9]+]] y: complex<f32>) -> complex<f64> [linkage=external] [abi=x86_win32 vectorcall(coerce<f64, f64>, coerce<f32, f32>) -> coerce<f64, f64>];
// IR-NEXT:     fn %[[VALUE_floats:[0-9]+]] @floats(%[[VALUE_a:[0-9]+]] a: f32, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f32, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_e:[0-9]+]] e: f32, %[[VALUE_f:[0-9]+]] f: f64, %[[VALUE_g:[0-9]+]] g: f32, %[[VALUE_h:[0-9]+]] h: f64) -> f64 [linkage=external] [abi=x86_win32 vectorcall(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_vectors:[0-9]+]] @vectors(%[[VALUE_a_2:[0-9]+]] a: vector<f32, 4> [c="v4f"], %[[VALUE_b_2:[0-9]+]] b: vector<f32, 4> [c="v4f"], %[[VALUE_c_2:[0-9]+]] c: vector<f32, 4> [c="v4f"], %[[VALUE_d_2:[0-9]+]] d: vector<f32, 4> [c="v4f"], %[[VALUE_e_2:[0-9]+]] e: vector<f32, 4> [c="v4f"], %[[VALUE_f_2:[0-9]+]] f: vector<f32, 4> [c="v4f"], %[[VALUE_g_2:[0-9]+]] g: vector<f32, 4> [c="v4f"]) -> vector<f32, 4> [linkage=external] [abi=x86_win32 vectorcall(direct, direct, direct, direct, direct, direct, byref<align=16>) -> direct] [c="v4f(v4f, v4f, v4f, v4f, v4f, v4f, v4f) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_late:[0-9]+]] @late(%[[VALUE_x_12:[0-9]+]] x: @type[[TYPE5]] [c="d4"], %[[VALUE_y_12:[0-9]+]] y: @type[[TYPE4]] [c="d2"], %[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64) -> void [linkage=external] [abi=x86_win32 vectorcall(byref<align=8>, coerce<f64, f64>, scalar, scalar, scalar) -> void] [c="void(d4, d2, double, double, double) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_exhaust:[0-9]+]] @exhaust(%[[VALUE_x_13:[0-9]+]] x: @type[[TYPE5]] [c="d4"], %[[VALUE_y_13:[0-9]+]] y: @type[[TYPE4]] [c="d2"], %[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_z:[0-9]+]] z: @type[[TYPE6]] [c="f2"]) -> void [linkage=external] [abi=x86_win32 vectorcall(coerce<f64, f64, f64, f64>, byref<align=8>, scalar, byref<align=4>) -> void] [c="void(d4, d2, double, f2) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_many:[0-9]+]] @many(%[[VALUE_a_5:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64, %[[VALUE_d_3:[0-9]+]] d: f64, %[[VALUE_e_3:[0-9]+]] e: f64, %[[VALUE_f_3:[0-9]+]] f: f64, %[[VALUE_g_3:[0-9]+]] g: f64, %[[VALUE_x_14:[0-9]+]] x: @type[[TYPE4]] [c="d2"]) -> void [linkage=external] [abi=x86_win32 vectorcall(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byref<align=8>) -> void] [c="void(double, double, double, double, double, double, double, d2) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_atomic:[0-9]+]] @atomic(%[[VALUE_a_6:[0-9]+]] a: atomic f64, %[[VALUE_x_15:[0-9]+]] x: @type[[TYPE4]] [c="d2"]) -> void [linkage=external] [abi=x86_win32 vectorcall(scalar, coerce<f64, f64>) -> void] [c="void(_Atomic double, d2) __attribute__((vectorcall))"];
// IR-NEXT:     fn %[[VALUE_ints:[0-9]+]] @ints(%[[VALUE_a_7:[0-9]+]] a: i64, %[[VALUE_b_5:[0-9]+]] b: i32, %[[VALUE_c_5:[0-9]+]] c: i8, %[[VALUE_d_4:[0-9]+]] d: bool, %[[VALUE_e_4:[0-9]+]] e: ptr<void>) -> i32 [linkage=external] [abi=x86_win32 vectorcall(scalar, scalar, scalar, scalar, scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_x_16:[0-9]+]] x: @type[[TYPE4]] [c="d2"], %[[VALUE_y_14:[0-9]+]] y: @type[[TYPE6]] [c="f2"]) -> @type[[TYPE4]] [linkage=external] [abi=x86_win32(native_c, native_c) -> native_c] [fallthrough=ub_if_used] [c="d2(d2, f2)"] {
// IR-NEXT:         call<void, abi=x86_win32 vectorcall(coerce<f64, f64, f64, f64>, byref<align=8>, scalar, byref<align=4>) -> void>(%[[VALUE_exhaust]], copy<@type[[TYPE5]], reason=arg>(read<@type[[TYPE5]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE5]], zero_fill=true>(field0 = int_to_float<f64>(const<i32>(0)))) [c="d4"]) [c="d4"], copy<@type[[TYPE4]], reason=arg>(read<@type[[TYPE4]]>(%[[VALUE_x_16]]) [c="d2"]) [c="d2"], int_to_float<f64>(const<i32>(1)), copy<@type[[TYPE6]], reason=arg>(read<@type[[TYPE6]]>(%[[VALUE_y_14]]) [c="f2"]) [c="f2"]);
// IR-NEXT:         return copy<@type[[TYPE4]], reason=return>(call<@type[[TYPE4]], abi=x86_win32 vectorcall(coerce<f64, f64>, scalar) -> coerce<f64, f64>>(%[[VALUE_fd2]], copy<@type[[TYPE4]], reason=arg>(read<@type[[TYPE4]]>(%[[VALUE_x_16]]) [c="d2"]) [c="d2"], const<i32>(1)) [c="d2"]) [c="d2"];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
