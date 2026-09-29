// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata -std=c23

// 6.5.15p6: a null pointer constant operand yields the other operand's type,
// which is why `merged_null` is `int *` and not `void *`.
void conditional(int c, int *p, const int *q, void *v) {
    typeof(c ? p : q) merged_qualifiers;
    typeof(c ? (int *)0 : (void *)0) merged_null;
    typeof(c ? 0 : p) null_constant;
    typeof(c ? v : p) merged_void;
    (void)merged_qualifiers;
    (void)merged_null;
    (void)null_constant;
    (void)merged_void;
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
// IR-NEXT:     fn %[[VALUE_conditional:[0-9]+]] @conditional(%[[VALUE_c:[0-9]+]] c: i32 [c="int"], %[[VALUE_p:[0-9]+]] p: ptr<i32> [c="int *"], %[[VALUE_q:[0-9]+]] q: ptr<const i32> [c="const int *"], %[[VALUE_v:[0-9]+]] v: ptr<void> [c="void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(int, int *, const int *, void *)"] {
// IR-NEXT:         let %[[VALUE_merged_qualifiers:[0-9]+]] merged_qualifiers: ptr<const i32> [storage=automatic] [c="typeof(c ? p : q)"] [c_canon="const int *"];
// IR-NEXT:         let %[[VALUE_merged_null:[0-9]+]] merged_null: ptr<i32> [storage=automatic] [c="typeof(c ? (cast)0 : (cast)0)"] [c_canon="int *"];
// IR-NEXT:         let %[[VALUE_null_constant:[0-9]+]] null_constant: ptr<i32> [storage=automatic] [c="typeof(c ? 0 : p)"] [c_canon="int *"];
// IR-NEXT:         let %[[VALUE_merged_void:[0-9]+]] merged_void: ptr<void> [storage=automatic] [c="typeof(c ? v : p)"] [c_canon="void *"];
// IR-NEXT:         read<ptr<const i32>>(%[[VALUE_merged_qualifiers]]);
// IR-NEXT:         read<ptr<i32>>(%[[VALUE_merged_null]]);
// IR-NEXT:         read<ptr<i32>>(%[[VALUE_null_constant]]);
// IR-NEXT:         read<ptr<void>>(%[[VALUE_merged_void]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
