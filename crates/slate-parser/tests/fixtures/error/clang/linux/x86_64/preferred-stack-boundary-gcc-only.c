// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -mpreferred-stack-boundary=4

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × invalid compiler argument `-mpreferred-stack-boundary=4`: unknown option
// SLATE-FILECHECK-END ERROR
