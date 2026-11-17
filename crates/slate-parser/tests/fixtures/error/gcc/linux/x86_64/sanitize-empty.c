int value;

// SLATE-FILECHECK-ARGS -fsanitize=
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=`: missing argument
// SLATE-FILECHECK-END PARSE
