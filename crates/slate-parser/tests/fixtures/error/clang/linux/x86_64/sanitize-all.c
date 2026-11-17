int value;

// SLATE-FILECHECK-ARGS -fsanitize=all
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=all`: unsupported argument `all`
// SLATE-FILECHECK-END PARSE
