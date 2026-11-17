int value;

// SLATE-FILECHECK-ARGS -fsanitize-coverage=trace-pc,0
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-coverage=trace-pc,0`: unsupported
// SLATE-FILECHECK-END PARSE
