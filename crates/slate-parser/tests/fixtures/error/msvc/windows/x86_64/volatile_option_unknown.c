// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /volatile:x
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/volatile:x`: expected one of iso, ms
// SLATE-FILECHECK-END DEFAULT
