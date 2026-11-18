// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir -fwide-exec-charset=UTF-16LE

int value(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `-fwide-exec-charset=UTF-16LE`: character set
// SLATE-FILECHECK-END DEFAULT
