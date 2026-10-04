// SLATE-FILECHECK-ARGS -masm=intel
// SLATE-FILECHECK-ERROR PARSE

int value;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-masm=intel`: unknown option for the msvc
// SLATE-FILECHECK-END PARSE
