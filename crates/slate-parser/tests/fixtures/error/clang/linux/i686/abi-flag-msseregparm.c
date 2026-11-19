int value;

// SLATE-FILECHECK-ARGS -msseregparm
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-msseregparm`: unknown option for the clang
// SLATE-FILECHECK-END PARSE
