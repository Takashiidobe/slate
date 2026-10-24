// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef struct { int a, b, c; } r3;
typedef struct { int a, b; } r2;
typedef struct { double a, b; } d2;
typedef struct { double a, b, c, d; } d4;
typedef struct { float a, b; } f2;
typedef struct { float a, b, c; } f3;
typedef struct { char a; } c1;
typedef struct { double a; } d1;
typedef struct { long long a, b; } l2;
typedef struct { } e0;
typedef float v4f __attribute__((vector_size(16)));
typedef struct { v4f a, b; } hv2;
typedef struct { v4f a; float b[4]; } mix;
typedef struct { float a; double b; } pad;
typedef union { float a; float b[2]; } u2;
typedef struct { float a[0]; float b; } za;
typedef struct { _Complex float c; float d; } cx;
r3 __vectorcall fr3(r3 x);
r2 __vectorcall fr2(r2 x);
d2 __vectorcall fd2(d2 x);
d4 __vectorcall fd4(d4 x);
f2 __vectorcall ff2(f2 x);
f3 __vectorcall ff3(f3 x);
c1 __vectorcall fc1(c1 x);
d1 __vectorcall fd1(d1 x);
l2 __vectorcall fl2(l2 x);
e0 __vectorcall fe0(e0 x);
hv2 __vectorcall fhv2(hv2 x);
mix __vectorcall fmix(mix x);
pad __vectorcall fpad(pad x);
u2 __vectorcall fu2(u2 x);
za __vectorcall fza(za x);
cx __vectorcall fcx(cx x);
v4f __vectorcall fv4f(v4f x);
long long __vectorcall fll(long long x);
_Complex double __vectorcall fcd(_Complex double x);
_Complex float __vectorcall fcf(_Complex float x);
double __vectorcall fdd(double x);
void __vectorcall exhaust(double a, double b, double c, double d, double e, d2 x, f2 y);
void __vectorcall hvas(d2 x, d2 y, d2 z, f2 w);
void __vectorcall position(int a, int b, int c, int d, int e, int f, double g, f2 h);
d2 use(d2 x, f2 y) { exhaust(1, 2, 3, 4, 5, x, y); return fd2(x); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
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
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type[[TYPE_r3:[0-9]+]] r3 = @type[[TYPE0]];
// IR-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_r2:[0-9]+]] r2 = @type[[TYPE1]];
// IR-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_d2:[0-9]+]] d2 = @type[[TYPE2]];
// IR-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:         field2 c: f64;
// IR-NEXT:         field3 d: f64;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// IR-NEXT:     type @type[[TYPE_d4:[0-9]+]] d4 = @type[[TYPE3]];
// IR-NEXT:     type @type[[TYPE4:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_f2:[0-9]+]] f2 = @type[[TYPE4]];
// IR-NEXT:     type @type[[TYPE5:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:         field2 c: f32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type[[TYPE_f3:[0-9]+]] f3 = @type[[TYPE5]];
// IR-NEXT:     type @type[[TYPE6:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_c1:[0-9]+]] c1 = @type[[TYPE6]];
// IR-NEXT:     type @type[[TYPE7:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_d1:[0-9]+]] d1 = @type[[TYPE7]];
// IR-NEXT:     type @type[[TYPE8:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_l2:[0-9]+]] l2 = @type[[TYPE8]];
// IR-NEXT:     type @type[[TYPE9:[0-9]+]] = struct {
// IR-NEXT:     } [size=4, align=1, offsets=[]];
// IR-NEXT:     type @type[[TYPE_e0:[0-9]+]] e0 = @type[[TYPE9]];
// IR-NEXT:     type @type[[TYPE_v4f:[0-9]+]] v4f = vector<f32, 4>;
// IR-NEXT:     type @type[[TYPE10:[0-9]+]] = struct {
// IR-NEXT:         field0 a: vector<f32, 4>;
// IR-NEXT:         field1 b: vector<f32, 4>;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type[[TYPE_hv2:[0-9]+]] hv2 = @type[[TYPE10]];
// IR-NEXT:     type @type[[TYPE11:[0-9]+]] = struct {
// IR-NEXT:         field0 a: vector<f32, 4>;
// IR-NEXT:         field1 b: array<f32, 4>;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type[[TYPE_mix:[0-9]+]] mix = @type[[TYPE11]];
// IR-NEXT:     type @type[[TYPE12:[0-9]+]] = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_pad:[0-9]+]] pad = @type[[TYPE12]];
// IR-NEXT:     type @type[[TYPE13:[0-9]+]] = union {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: array<f32, 2>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_u2:[0-9]+]] u2 = @type[[TYPE13]];
// IR-NEXT:     type @type[[TYPE14:[0-9]+]] = struct {
// IR-NEXT:         field0 a: array<f32, 0>;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_za:[0-9]+]] za = @type[[TYPE14]];
// IR-NEXT:     type @type[[TYPE15:[0-9]+]] = struct {
// IR-NEXT:         field0 c: complex<f32>;
// IR-NEXT:         field1 d: f32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_cx:[0-9]+]] cx = @type[[TYPE15]];
// IR-NEXT:     fn %[[VALUE_fr3:[0-9]+]] @fr3(%[[VALUE_x:[0-9]+]] x: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fr2:[0-9]+]] @fr2(%[[VALUE_x_2:[0-9]+]] x: @type[[TYPE1]]) -> @type[[TYPE1]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fd2:[0-9]+]] @fd2(%[[VALUE_x_3:[0-9]+]] x: @type[[TYPE2]]) -> @type[[TYPE2]] [linkage=external] [abi=win64 vectorcall(coerce<f64, f64>) -> coerce<f64, f64>];
// IR-NEXT:     fn %[[VALUE_fd4:[0-9]+]] @fd4(%[[VALUE_x_4:[0-9]+]] x: @type[[TYPE3]]) -> @type[[TYPE3]] [linkage=external] [abi=win64 vectorcall(coerce<f64, f64, f64, f64>) -> coerce<f64, f64, f64, f64>];
// IR-NEXT:     fn %[[VALUE_ff2:[0-9]+]] @ff2(%[[VALUE_x_5:[0-9]+]] x: @type[[TYPE4]]) -> @type[[TYPE4]] [linkage=external] [abi=win64 vectorcall(coerce<f32, f32>) -> coerce<f32, f32>];
// IR-NEXT:     fn %[[VALUE_ff3:[0-9]+]] @ff3(%[[VALUE_x_6:[0-9]+]] x: @type[[TYPE5]]) -> @type[[TYPE5]] [linkage=external] [abi=win64 vectorcall(coerce<f32, f32, f32>) -> coerce<f32, f32, f32>];
// IR-NEXT:     fn %[[VALUE_fc1:[0-9]+]] @fc1(%[[VALUE_x_7:[0-9]+]] x: @type[[TYPE6]]) -> @type[[TYPE6]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fd1:[0-9]+]] @fd1(%[[VALUE_x_8:[0-9]+]] x: @type[[TYPE7]]) -> @type[[TYPE7]] [linkage=external] [abi=win64 vectorcall(coerce<f64>) -> coerce<f64>];
// IR-NEXT:     fn %[[VALUE_fl2:[0-9]+]] @fl2(%[[VALUE_x_9:[0-9]+]] x: @type[[TYPE8]]) -> @type[[TYPE8]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fe0:[0-9]+]] @fe0(%[[VALUE_x_10:[0-9]+]] x: @type[[TYPE9]]) -> @type[[TYPE9]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fhv2:[0-9]+]] @fhv2(%[[VALUE_x_11:[0-9]+]] x: @type[[TYPE10]]) -> @type[[TYPE10]] [linkage=external] [abi=win64 vectorcall(coerce<vector<f32, 4>, vector<f32, 4>>) -> coerce<vector<f32, 4>, vector<f32, 4>>];
// IR-NEXT:     fn %[[VALUE_fmix:[0-9]+]] @fmix(%[[VALUE_x_12:[0-9]+]] x: @type[[TYPE11]]) -> @type[[TYPE11]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fpad:[0-9]+]] @fpad(%[[VALUE_x_13:[0-9]+]] x: @type[[TYPE12]]) -> @type[[TYPE12]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fu2:[0-9]+]] @fu2(%[[VALUE_x_14:[0-9]+]] x: @type[[TYPE13]]) -> @type[[TYPE13]] [linkage=external] [abi=win64 vectorcall(coerce<f32, f32>) -> coerce<f32, f32>];
// IR-NEXT:     fn %[[VALUE_fza:[0-9]+]] @fza(%[[VALUE_x_15:[0-9]+]] x: @type[[TYPE14]]) -> @type[[TYPE14]] [linkage=external] [abi=win64 vectorcall(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_fcx:[0-9]+]] @fcx(%[[VALUE_x_16:[0-9]+]] x: @type[[TYPE15]]) -> @type[[TYPE15]] [linkage=external] [abi=win64 vectorcall(coerce<f32, f32, f32>) -> coerce<f32, f32, f32>];
// IR-NEXT:     fn %[[VALUE_fv4f:[0-9]+]] @fv4f(%[[VALUE_x_17:[0-9]+]] x: vector<f32, 4>) -> vector<f32, 4> [linkage=external] [abi=win64 vectorcall(direct) -> direct];
// IR-NEXT:     fn %[[VALUE_fll:[0-9]+]] @fll(%[[VALUE_x_18:[0-9]+]] x: i64) -> i64 [linkage=external] [abi=win64 vectorcall(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_fcd:[0-9]+]] @fcd(%[[VALUE_x_19:[0-9]+]] x: complex<f64>) -> complex<f64> [linkage=external] [abi=win64 vectorcall(coerce<f64, f64>) -> coerce<f64, f64>];
// IR-NEXT:     fn %[[VALUE_fcf:[0-9]+]] @fcf(%[[VALUE_x_20:[0-9]+]] x: complex<f32>) -> complex<f32> [linkage=external] [abi=win64 vectorcall(coerce<f32, f32>) -> coerce<f32, f32>];
// IR-NEXT:     fn %[[VALUE_fdd:[0-9]+]] @fdd(%[[VALUE_x_21:[0-9]+]] x: f64) -> f64 [linkage=external] [abi=win64 vectorcall(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_exhaust:[0-9]+]] @exhaust(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_e:[0-9]+]] e: f64, %[[VALUE_x_22:[0-9]+]] x: @type[[TYPE2]], %[[VALUE_y:[0-9]+]] y: @type[[TYPE4]]) -> void [linkage=external] [abi=win64 vectorcall(scalar, scalar, scalar, scalar, scalar, native_c, byref<align=4>) -> void];
// IR-NEXT:     fn %[[VALUE_hvas:[0-9]+]] @hvas(%[[VALUE_x_23:[0-9]+]] x: @type[[TYPE2]], %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE2]], %[[VALUE_z:[0-9]+]] z: @type[[TYPE2]], %[[VALUE_w:[0-9]+]] w: @type[[TYPE4]]) -> void [linkage=external] [abi=win64 vectorcall(coerce<f64, f64>, coerce<f64, f64>, coerce<f64, f64>, byref<align=4>) -> void];
// IR-NEXT:     fn %[[VALUE_position:[0-9]+]] @position(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32, %[[VALUE_c_2:[0-9]+]] c: i32, %[[VALUE_d_2:[0-9]+]] d: i32, %[[VALUE_e_2:[0-9]+]] e: i32, %[[VALUE_f:[0-9]+]] f: i32, %[[VALUE_g:[0-9]+]] g: f64, %[[VALUE_h:[0-9]+]] h: @type[[TYPE4]]) -> void [linkage=external] [abi=win64 vectorcall(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f32, f32>) -> void];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_x_24:[0-9]+]] x: @type[[TYPE2]], %[[VALUE_y_3:[0-9]+]] y: @type[[TYPE4]]) -> @type[[TYPE2]] [linkage=external] [abi=win64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         call<void, abi=win64 vectorcall(scalar, scalar, scalar, scalar, scalar, native_c, byref<align=4>) -> void>(%[[VALUE_exhaust]], int_to_float<f64>(const<i32>(1)), int_to_float<f64>(const<i32>(2)), int_to_float<f64>(const<i32>(3)), int_to_float<f64>(const<i32>(4)), int_to_float<f64>(const<i32>(5)), copy<@type[[TYPE2]], reason=arg>(read<@type[[TYPE2]]>(%[[VALUE_x_24]])), copy<@type[[TYPE4]], reason=arg>(read<@type[[TYPE4]]>(%[[VALUE_y_3]])));
// IR-NEXT:         return copy<@type[[TYPE2]], reason=return>(call<@type[[TYPE2]], abi=win64 vectorcall(coerce<f64, f64>) -> coerce<f64, f64>>(%[[VALUE_fd2]], copy<@type[[TYPE2]], reason=arg>(read<@type[[TYPE2]]>(%[[VALUE_x_24]]))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
