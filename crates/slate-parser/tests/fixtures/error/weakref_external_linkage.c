// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int target;
int reference __attribute__((weakref("target")));

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × invalid in this context: weakref without internal linkage
// IR: ╭─[tests/fixtures/error/weakref_external_linkage.c:3:1]
// IR: 2 │ int target;
// IR: 3 │ int reference __attribute__((weakref("target")));
// IR: · ─────────────────────────────────────────────────
// IR: 4 │
// IR: ╰────
// SLATE-FILECHECK-END IR
