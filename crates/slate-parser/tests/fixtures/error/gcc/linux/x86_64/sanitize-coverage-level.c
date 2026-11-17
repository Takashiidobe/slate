int value;

// SLATE-FILECHECK-ARGS -fsanitize-coverage=0
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-coverage=0`: unsupported argument
// SLATE-FILECHECK-END PARSE
