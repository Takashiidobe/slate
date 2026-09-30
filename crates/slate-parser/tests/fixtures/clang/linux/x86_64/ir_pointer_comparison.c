// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int compared(int *p, unsigned *u, long *l, void *v, int x) {
    return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
}
int unevaluated(int *p) {
    return sizeof(p == 2);
}

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wpointer-integer-compare
// WARN: ⚠ comparison between pointer and integer
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_comparison.c:3:13]
// WARN: 2 │ int compared(int *p, unsigned *u, long *l, void *v, int x) {
// WARN: 3 │     return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
// WARN: ·             ──────
// WARN: 4 │ }
// WARN: ╰────
// WARN: -Wcompare-distinct-pointer-types
// WARN: ⚠ comparison of distinct pointer types
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_comparison.c:3:24]
// WARN: 2 │ int compared(int *p, unsigned *u, long *l, void *v, int x) {
// WARN: 3 │     return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
// WARN: ·                        ──────
// WARN: 4 │ }
// WARN: ╰────
// WARN: -Wcompare-distinct-pointer-types
// WARN: ⚠ comparison of distinct pointer types
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_comparison.c:3:35]
// WARN: 2 │ int compared(int *p, unsigned *u, long *l, void *v, int x) {
// WARN: 3 │     return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
// WARN: ·                                   ─────
// WARN: 4 │ }
// WARN: ╰────
// WARN: -Wpointer-integer-compare
// WARN: ⚠ comparison between pointer and integer
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_pointer_comparison.c:6:19]
// WARN: 5 │ int unevaluated(int *p) {
// WARN: 6 │     return sizeof(p == 2);
// WARN: ·                   ──────
// WARN: 7 │ }
// WARN: ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f80;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f80 [size=16, align=16];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_compared:[0-9]+]] @compared(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_u:[0-9]+]] u: ptr<u32>, %[[VALUE_l:[0-9]+]] l: ptr<i64>, %[[VALUE_v:[0-9]+]] v: ptr<void>, %[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p]]), int_to_ptr<ptr<i32>, reason=usual_arith>(read<i32>(%[[VALUE_x]])))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p]]), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<u32>>(%[[VALUE_u]]))))), from_bool<i32, reason=promotion>(lt<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p]]), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<i64>>(%[[VALUE_l]]))))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p]]), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<void>>(%[[VALUE_v]]))))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p]]), null<ptr<i32>>)));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_unevaluated:[0-9]+]] @unevaluated(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
