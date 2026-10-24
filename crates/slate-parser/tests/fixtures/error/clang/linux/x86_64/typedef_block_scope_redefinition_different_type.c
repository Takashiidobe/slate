// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void f(void) {
    typedef int U;
    typedef long U;
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × typedef redefinition with different types
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/typedef_block_scope_redefinition_different_type.c:4:18]
// IR:  3 │     typedef int U;
// IR:  4 │     typedef long U;
// IR:    ·                  ─
// IR:  5 │ }
// IR:    ╰────
// SLATE-FILECHECK-END IR
