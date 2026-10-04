int value;

// SLATE-FILECHECK-ARGS -fcf-protection=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fcf-protection=bogus`: unknown control-flow
// SLATE-FILECHECK-END PARSE
