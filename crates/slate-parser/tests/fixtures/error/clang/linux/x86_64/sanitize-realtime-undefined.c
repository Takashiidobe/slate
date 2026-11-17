int value;

// SLATE-FILECHECK-ARGS -fsanitize=realtime -fsanitize=undefined
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=realtime`: not allowed with
// SLATE-FILECHECK-END PARSE
