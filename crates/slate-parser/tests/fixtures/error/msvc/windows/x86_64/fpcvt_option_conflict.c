// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /fpcvt:IA /fpcvt:BC
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/fpcvt:BC`: incompatible with /fpcvt:IA
// SLATE-FILECHECK-END DEFAULT
