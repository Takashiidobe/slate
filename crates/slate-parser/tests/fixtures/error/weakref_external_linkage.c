// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int target;
int reference __attribute__((weakref("target")));

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × invalid in this context: weakref without internal linkage
// SLATE-FILECHECK-END IR
