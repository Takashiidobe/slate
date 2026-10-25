// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS /fsanitize=Address
int option_macros;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/fsanitize=Address`: expected one of address,
// SLATE-FILECHECK-END DEFAULT
