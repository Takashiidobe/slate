// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -march=haswell

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × all rules failed: target ISA options: x86 architecture `haswell` is only
// SLATE-FILECHECK-END ERROR
