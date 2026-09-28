// SLATE-FILECHECK-ARGS --dump-ir

int address_parameter(register int parameter) {
    return *(&parameter);
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: address of register variable requested
// SEMANTIC: ╭─[tests/fixtures/error/register-parameter-address.c:3:14]
// SEMANTIC: 2 │ int address_parameter(register int parameter) {
// SEMANTIC: 3 │     return *(&parameter);
// SEMANTIC: ·              ──────────
// SEMANTIC: 4 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
