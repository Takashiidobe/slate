// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata --show-comments
/* parser-file-comment */
struct Value {
    /* parser-field-comment */
    int n;
};
int value(int n) {
    /* parser-local-comment */
    struct Value v = {n};
    /* parser-return-comment */
    return v.n;
    /* parser-trailing-comment */
}

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
// IR-NEXT:     comment leading "/* parser-file-comment */" [spelling=[[#FILE0:]]:0+25, expansion={{[0-9]+}}:0+25];
// IR-NEXT:     type @type[[TYPE_Value:[0-9]+]] Value = struct {
// IR-NEXT:         comment leading "/* parser-field-comment */" [spelling=[[#FILE0]]:45+26, expansion=[[#FILE0]]:45+26];
// IR-NEXT:         field0 n: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_value:[0-9]+]] @value(%[[VALUE_n:[0-9]+]] n: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         comment leading "/* parser-local-comment */" [spelling=[[#FILE0]]:109+26, expansion=[[#FILE0]]:109+26];
// IR-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE_Value]] [storage=automatic] = aggregate<@type[[TYPE_Value]], zero_fill=false>(field0 = read<i32>(%[[VALUE_n]])) [c="struct Value"];
// IR-NEXT:         comment leading "/* parser-return-comment */" [spelling=[[#FILE0]]:166+27, expansion=[[#FILE0]]:166+27];
// IR-NEXT:         return read<i32>(field0(%[[VALUE_v]]));
// IR-NEXT:         comment detached "/* parser-trailing-comment */" [spelling=[[#FILE0]]:214+29, expansion=[[#FILE0]]:214+29];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
