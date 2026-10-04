int value;

// SLATE-FILECHECK-ARGS -fstack-protector-explicit
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fstack-protector-explicit`: unknown option for
// SLATE-FILECHECK-END PARSE
