int value;

// SLATE-FILECHECK-ARGS -mabi=ms
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-mabi=ms`: not supported for target 'aarch64-
// SLATE-FILECHECK-END PARSE
