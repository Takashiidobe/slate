// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef unsigned int T;
typedef int T;

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × typedef redefinition with different types
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/typedef_redefinition_signedness.c:3:13]
// IR: 2 │ typedef unsigned int T;
// IR: 3 │ typedef int T;
// IR: ·             ─
// IR: ╰────
// SLATE-FILECHECK-END IR
