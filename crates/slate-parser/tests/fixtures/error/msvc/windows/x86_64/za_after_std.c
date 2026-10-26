// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /std:clatest /Za
int strict_ansi;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/Za`: incompatible with /std:clatest
// SLATE-FILECHECK-END DEFAULT
