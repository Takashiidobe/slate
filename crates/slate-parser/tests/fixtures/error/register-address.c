// SLATE-FILECHECK-ARGS --dump-ir

int address_local(void) {
    register int local;
    return *(&local);
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: address of register variable requested
// SEMANTIC: ╭─[tests/fixtures/error/register-address.c:2:1]
// SEMANTIC: 1 │
// SEMANTIC: 2 │ ╭─▶ int address_local(void) {
// SEMANTIC: 3 │ │       register int local;
// SEMANTIC: 4 │ │       return *(&local);
// SEMANTIC: 5 │ ╰─▶ }
// SEMANTIC: 6 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
