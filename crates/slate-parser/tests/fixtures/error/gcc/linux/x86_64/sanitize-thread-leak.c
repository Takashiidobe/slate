int value;

// SLATE-FILECHECK-ARGS -fsanitize=thread,leak
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=thread`: not allowed with
// SLATE-FILECHECK-END PARSE
