// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

static __declspec(selectany) int chosen = 1;

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × invalid in this context: selectany without external linkage
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/selectany_internal_linkage.c:2:1]
// IR: 1 │
// IR: 2 │ static __declspec(selectany) int chosen = 1;
// IR: · ────────────────────────────────────────────
// IR: 3 │
// IR: ╰────
// SLATE-FILECHECK-END IR
