int value;

// SLATE-FILECHECK-ARGS -mcmodel=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-mcmodel=bogus`: unknown code model
// SLATE-FILECHECK-END PARSE
