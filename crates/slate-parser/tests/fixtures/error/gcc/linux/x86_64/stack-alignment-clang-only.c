// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -mstack-alignment=16

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × invalid compiler argument `-mstack-alignment=16`: unknown option for the
// SLATE-FILECHECK-END ERROR
