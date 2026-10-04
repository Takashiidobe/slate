int value;

// SLATE-FILECHECK-ARGS -mllvm
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-mllvm`: missing value
// SLATE-FILECHECK-END PARSE
