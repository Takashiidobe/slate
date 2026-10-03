// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -mpclmul

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × all rules failed: target ISA options: x86 feature `pclmul` is only
// SLATE-FILECHECK-END ERROR
