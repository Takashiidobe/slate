int value;

// SLATE-FILECHECK-ARGS -ftrivial-auto-var-init=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ftrivial-auto-var-init=bogus`: invalid
// SLATE-FILECHECK-END PARSE
