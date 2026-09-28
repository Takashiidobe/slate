// SLATE-FILECHECK-ERROR RESOLVE
// SLATE-FILECHECK-ARGS --dump-ir-names

int f(void) {
    return missing;
}

// SLATE-FILECHECK-BEGIN RESOLVE
// RESOLVE: Error:   × unresolved ordinary name `missing`
// SLATE-FILECHECK-END RESOLVE
