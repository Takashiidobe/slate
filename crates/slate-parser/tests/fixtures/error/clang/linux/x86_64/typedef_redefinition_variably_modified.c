// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void f(int n) {
    typedef int A[n];
    typedef int A[n];
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × redefinition of typedef for variably-modified type
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/typedef_redefinition_variably_modified.c:4:17]
// IR:  3 │     typedef int A[n];
// IR:  4 │     typedef int A[n];
// IR:    ·                 ────
// IR:  5 │ }
// IR:    ╰────
// SLATE-FILECHECK-END IR
