// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -fasm-blocks

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × invalid compiler argument `-fasm-blocks`: unknown option for the gcc
// SLATE-FILECHECK-END ERROR
