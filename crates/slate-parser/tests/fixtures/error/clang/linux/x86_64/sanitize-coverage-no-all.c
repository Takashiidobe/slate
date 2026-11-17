int value;

// SLATE-FILECHECK-ARGS -fno-sanitize-coverage=all
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fno-sanitize-coverage=all`: unsupported
// SLATE-FILECHECK-END PARSE
