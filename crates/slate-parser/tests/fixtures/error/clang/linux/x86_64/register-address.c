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
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/register-address.c:4:14]
// SEMANTIC: 3 │     register int local;
// SEMANTIC: 4 │     return *(&local);
// SEMANTIC: ·              ──────
// SEMANTIC: 5 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
