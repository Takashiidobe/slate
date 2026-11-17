int value;

// SLATE-FILECHECK-ARGS -fsanitize-coverage=func -fsanitize-coverage=bb
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fsanitize-coverage=func`: not allowed with
// SLATE-FILECHECK-END PARSE
