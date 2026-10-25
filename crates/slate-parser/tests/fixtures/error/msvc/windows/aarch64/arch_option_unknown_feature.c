// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /arch:armv8.0+bogus
int arch_option;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/arch:armv8.0+bogus`: expected one of lse,
// SLATE-FILECHECK-END DEFAULT
