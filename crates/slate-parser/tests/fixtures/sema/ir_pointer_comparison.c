// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int compared(int *p, unsigned *u, long *l, void *v, int x) {
    return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
}

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wpointer-integer-compare
// WARN: ⚠ comparison between pointer and integer
// WARN: ╭─[tests/fixtures/sema/ir_pointer_comparison.c:3:13]
// WARN: 2 │ int compared(int *p, unsigned *u, long *l, void *v, int x) {
// WARN: 3 │     return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
// WARN: ·             ──────
// WARN: 4 │ }
// WARN: ╰────
// WARN: -Wcompare-distinct-pointer-types
// WARN: ⚠ comparison of distinct pointer types
// WARN: ╭─[tests/fixtures/sema/ir_pointer_comparison.c:3:24]
// WARN: 2 │ int compared(int *p, unsigned *u, long *l, void *v, int x) {
// WARN: 3 │     return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
// WARN: ·                        ──────
// WARN: 4 │ }
// WARN: ╰────
// WARN: -Wcompare-distinct-pointer-types
// WARN: ⚠ comparison of distinct pointer types
// WARN: ╭─[tests/fixtures/sema/ir_pointer_comparison.c:3:35]
// WARN: 2 │ int compared(int *p, unsigned *u, long *l, void *v, int x) {
// WARN: 3 │     return (p == x) + (p == u) + (p < l) + (p == v) + (p == 0);
// WARN: ·                                   ─────
// WARN: 4 │ }
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
// IR-WARN-NEXT:     fn %0 @compared(%1 p: ptr<i32>, %2 u: ptr<u32>, %3 l: ptr<i64>, %4 v: ptr<void>, %5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%1), int_to_ptr<ptr<i32>, reason=usual_arith>(read<i32>(%5)))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%1), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<u32>>(%2))))), from_bool<i32, reason=promotion>(lt<ptr<i32>>(read<ptr<i32>>(%1), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<i64>>(%3))))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%1), pointer_cast<ptr<i32>, reason=usual_arith>(read<ptr<void>>(%4))))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%1), null<ptr<i32>>)));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
