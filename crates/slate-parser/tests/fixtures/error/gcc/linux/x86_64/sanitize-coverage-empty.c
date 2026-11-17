int value;

// SLATE-FILECHECK-ARGS -fsanitize-coverage=
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-coverage=`: missing argument
// SLATE-FILECHECK-END PARSE
