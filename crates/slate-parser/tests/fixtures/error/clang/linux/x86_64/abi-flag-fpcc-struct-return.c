int value;

// SLATE-FILECHECK-ARGS -fpcc-struct-return
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fpcc-struct-return`: not supported for target
// SLATE-FILECHECK-END PARSE
