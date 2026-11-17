int value;

// SLATE-FILECHECK-ARGS -fsanitize=bounds-strict
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=bounds-strict`: unsupported argument
// SLATE-FILECHECK-END PARSE
