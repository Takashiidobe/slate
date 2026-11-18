// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir -fexec-charset=IBM1047

int value(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `-fexec-charset=IBM1047`: invalid value
// SLATE-FILECHECK-END DEFAULT
