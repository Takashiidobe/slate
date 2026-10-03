// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -march=pentium4

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × all rules failed: target ISA options: CPU `pentium4` does not support 64-
// SLATE-FILECHECK-END ERROR
