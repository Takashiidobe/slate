// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros

#define ADD(a, b) ((a) + (b))
#define DIV(a, b) ((a) / (b))

double rounded_once(void) {
    double r = ADD(1.0, 0x1.002p-53);
    return r;
}

double third(void) {
    double r = DIV(1.0, 3.0);
    return r;
}

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
// IR-NEXT:     fn %[[VALUE_rounded_once:[0-9]+]] @rounded_once() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: f64 [storage=automatic] = macro_value<f64>(#[[#NODE0:]], statements=0, const<f64>(1.0000000000000002) [macro=ADD#0 body]) [macro=ADD#0 body];
// IR-NEXT:         return read<f64>(%[[VALUE_r]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_third:[0-9]+]] @third() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: f64 [storage=automatic] = macro_value<f64>(#[[#NODE1:]], statements=0, const<f64>(0.3333333333333333) [macro=DIV#1 body]) [macro=DIV#1 body];
// IR-NEXT:         return read<f64>(%[[VALUE_r_2]]);
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE0]] ADD(a, b) [defined=7:1] = "( ( a ) + ( b ) )";
// IR-NEXT:     macro_definition #[[#NODE1]] DIV(a, b) [defined=7:2] = "( ( a ) / ( b ) )";
// IR-NEXT:     macro_expansion #[[#NODE0]] ADD [definition=#[[#NODE0]]] [site=7:105+21] [arguments=109+3, 114+11] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] DIV [definition=#[[#NODE1]]] [site=7:181+13] [arguments=185+3, 190+3] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
