// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir -fexec-charset=IBM930

int value(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `-fexec-charset=IBM930`: character set 'IBM930'
// SLATE-FILECHECK-END DEFAULT
