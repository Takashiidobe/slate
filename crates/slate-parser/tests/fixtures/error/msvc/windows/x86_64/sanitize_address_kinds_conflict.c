// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /fsanitize=address /fsanitize=kernel-address
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/fsanitize=kernel-address`: incompatible with /
// SLATE-FILECHECK-END DEFAULT
