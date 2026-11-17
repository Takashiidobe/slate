int value;

// SLATE-FILECHECK-ARGS -fsanitize-coverage=edge
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-coverage=edge`: unsupported argument
// SLATE-FILECHECK-END PARSE
