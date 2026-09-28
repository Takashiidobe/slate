// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct empty {};
struct chars { char a, b, c; };
struct packed { char c; int i; } __attribute__((packed));

struct empty empty(struct empty value) { return value; }
struct chars chars(struct chars value) { return value; }
struct packed packed(struct packed value) { return value; }
_Complex char complex_char(_Complex char value) { return value; }
_Complex short complex_short(_Complex short value) { return value; }
_Complex int complex_int(_Complex int value) { return value; }
_Complex float complex_float(_Complex float value) { return value; }
_Complex double complex_double(_Complex double value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 empty = struct {
// IR-NEXT:     } [size=0, align=1, offsets=[]];
// IR-NEXT:     type @type1 chars = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i8;
// IR-NEXT:         field2 c: i8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     type @type2 packed = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     fn %3 @empty(%4 value: @type0) -> @type0 [linkage=external] [abi=x86_cdecl(native_c) -> sret<align=1>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%4));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @chars(%6 value: @type1) -> @type1 [linkage=external] [abi=x86_cdecl(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%6));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @packed(%8 value: @type2) -> @type2 [linkage=external] [abi=x86_cdecl(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%8));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @complex_char(%10 value: complex<i8>) -> complex<i8> [linkage=external] [abi=x86_cdecl(native_c) -> coerce<i16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<i8>>(%10);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @complex_short(%12 value: complex<i16>) -> complex<i16> [linkage=external] [abi=x86_cdecl(native_c) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<i16>>(%12);
// IR-NEXT:     }
// IR-NEXT:     fn %13 @complex_int(%14 value: complex<i32>) -> complex<i32> [linkage=external] [abi=x86_cdecl(native_c) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<i32>>(%14);
// IR-NEXT:     }
// IR-NEXT:     fn %15 @complex_float(%16 value: complex<f32>) -> complex<f32> [linkage=external] [abi=x86_cdecl(native_c) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%16);
// IR-NEXT:     }
// IR-NEXT:     fn %17 @complex_double(%18 value: complex<f64>) -> complex<f64> [linkage=external] [abi=x86_cdecl(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%18);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
