// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int plain(int a);
int __attribute__((ms_abi)) plain(int a);

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × function redeclared with a different calling convention
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/ms_abi_redeclaration.c:3:29]
// IR:  2 │ int plain(int a);
// IR:  3 │ int __attribute__((ms_abi)) plain(int a);
// IR:    ·                             ────────────
// IR:  4 │
// IR:    ╰────
// SLATE-FILECHECK-END IR
