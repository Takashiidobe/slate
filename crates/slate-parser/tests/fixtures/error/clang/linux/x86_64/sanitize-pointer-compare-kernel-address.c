int value;

// SLATE-FILECHECK-ARGS -fsanitize=pointer-compare -fsanitize=kernel-address
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize=pointer-compare`: only allowed with
// SLATE-FILECHECK-END PARSE
