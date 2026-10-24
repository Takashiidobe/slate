// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

struct S { int a : UNDEFINED_WIDTH; int b; };
int get(struct S *s) { return s->b; }

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × nonconstant or unknown identifier
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/record_definition_failure.c:2:1]
// IR:  1 │
// IR:  2 │ struct S { int a : UNDEFINED_WIDTH; int b; };
// IR:    · ─────────────────────────────────────────────
// IR:  3 │ int get(struct S *s) { return s->b; }
// IR:    ╰────
// IR: Error:
// IR:   × member of incomplete or non-record
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/record_definition_failure.c:3:31]
// IR:  2 │ struct S { int a : UNDEFINED_WIDTH; int b; };
// IR:  3 │ int get(struct S *s) { return s->b; }
// IR:    ·                               ────
// IR:  4 │
// IR:    ╰────
// SLATE-FILECHECK-END IR
