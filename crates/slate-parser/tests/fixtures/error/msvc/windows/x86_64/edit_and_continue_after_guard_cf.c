// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /guard:cf /ZI
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/ZI`: incompatible with /guard:cf
// SLATE-FILECHECK-END DEFAULT
