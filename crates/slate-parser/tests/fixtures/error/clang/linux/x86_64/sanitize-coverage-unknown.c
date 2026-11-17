int value;

// SLATE-FILECHECK-ARGS -fsanitize-coverage=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-coverage=bogus`: unsupported
// SLATE-FILECHECK-END PARSE
