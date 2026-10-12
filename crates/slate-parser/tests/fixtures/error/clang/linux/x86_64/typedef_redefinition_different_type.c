// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef unsigned int T;
typedef unsigned long long T;

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × typedef redefinition with different types
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/typedef_redefinition_different_type.c:3:28]
// IR: 2 │ typedef unsigned int T;
// IR: 3 │ typedef unsigned long long T;
// IR: ·                            ─
// IR: ╰────
// SLATE-FILECHECK-END IR
