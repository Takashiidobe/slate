int value;

// SLATE-FILECHECK-ARGS -fno-stack-protector-all
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fno-stack-protector-all`: unknown option
// SLATE-FILECHECK-END PARSE
