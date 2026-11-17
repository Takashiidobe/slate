int value;

// SLATE-FILECHECK-ARGS -fsanitize=address,thread
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=address`: not allowed with
// SLATE-FILECHECK-END PARSE
