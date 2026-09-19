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
