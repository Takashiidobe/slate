// SLATE-FILECHECK-ARGS --dump-ir

int address_local(void) {
    register int local;
    return *(&local);
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: address of register variable requested
// SLATE-FILECHECK-END SEMANTIC
