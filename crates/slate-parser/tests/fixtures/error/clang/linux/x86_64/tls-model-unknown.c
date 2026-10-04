int value;

// SLATE-FILECHECK-ARGS -ftls-model=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ftls-model=bogus`: expected one of global-
// SLATE-FILECHECK-END PARSE
