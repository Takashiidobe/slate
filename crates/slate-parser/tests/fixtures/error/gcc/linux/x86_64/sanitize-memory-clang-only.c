int value;

// SLATE-FILECHECK-ARGS -fsanitize=memory
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=memory`: unsupported argument
// SLATE-FILECHECK-END PARSE
