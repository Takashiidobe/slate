// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /kernel /arch:IA32 /arch:SSE2
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/arch:SSE2`: incompatible with /kernel
// SLATE-FILECHECK-END DEFAULT
