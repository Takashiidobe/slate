// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int n = 8;
_Alignas(n) int object;
typedef int alias __attribute__((aligned(n)));
struct fields {
  char field __attribute__((aligned(n)));
};

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × nonconstant or unknown identifier
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/alignment_operand_nonconstant.c:3:17]
// IR: 2 │ int n = 8;
// IR: 3 │ _Alignas(n) int object;
// IR: ·                 ──────
// IR: 4 │ typedef int alias __attribute__((aligned(n)));
// IR: ╰────
// IR: Error:
// IR: × nonconstant or unknown identifier
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/alignment_operand_nonconstant.c:4:13]
// IR: 3 │ _Alignas(n) int object;
// IR: 4 │ typedef int alias __attribute__((aligned(n)));
// IR: ·             ─────────────────────────────────
// IR: 5 │ struct fields {
// IR: ╰────
// IR: Error:
// IR: × nonconstant or unknown identifier
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/alignment_operand_nonconstant.c:5:1]
// IR: 4 │     typedef int alias __attribute__((aligned(n)));
// IR: 5 │ ╭─▶ struct fields {
// IR: 6 │ │     char field __attribute__((aligned(n)));
// IR: 7 │ ╰─▶ };
// IR: 8 │
// IR: ╰────
// SLATE-FILECHECK-END IR
