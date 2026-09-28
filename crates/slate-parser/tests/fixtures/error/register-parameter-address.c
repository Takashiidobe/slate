// SLATE-FILECHECK-ARGS --dump-ir

int address_parameter(register int parameter) {
    return *(&parameter);
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: address of register variable requested
// SEMANTIC: ╭─[tests/fixtures/error/register-parameter-address.c:2:1]
// SEMANTIC: 1 │
// SEMANTIC: 2 │ ╭─▶ int address_parameter(register int parameter) {
// SEMANTIC: 3 │ │       return *(&parameter);
// SEMANTIC: 4 │ ╰─▶ }
// SEMANTIC: 5 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
