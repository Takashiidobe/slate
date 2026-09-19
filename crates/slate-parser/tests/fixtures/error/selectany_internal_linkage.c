// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

static __declspec(selectany) int chosen = 1;

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × invalid in this context: selectany without external linkage
// SLATE-FILECHECK-END IR
