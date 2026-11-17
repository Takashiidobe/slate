int value;

// SLATE-FILECHECK-ARGS -fsanitize-minimal-runtime
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-minimal-runtime`: unknown option for
// SLATE-FILECHECK-END PARSE
