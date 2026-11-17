int value;

// SLATE-FILECHECK-ARGS -fno-sanitize-recover=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fno-sanitize-recover=bogus`: unsupported
// SLATE-FILECHECK-END PARSE
