// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -mavx

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × invalid compiler argument `-mavx`: unknown option for the msvc flavor
// SLATE-FILECHECK-END ERROR
