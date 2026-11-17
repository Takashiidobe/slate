int value;

// SLATE-FILECHECK-ARGS -fsanitize=shadow-call-stack
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=shadow-call-stack`: not supported on
// SLATE-FILECHECK-END PARSE
