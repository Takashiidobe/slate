// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /ZI /fsanitize=address
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/fsanitize=address`: incompatible with /ZI
// SLATE-FILECHECK-END DEFAULT
