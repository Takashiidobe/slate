// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /Gz /Gr

int value(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/Gr`: incompatible with /Gz
// SLATE-FILECHECK-END DEFAULT
