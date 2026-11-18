int value;

// SLATE-FILECHECK-ARGS -freg-struct-return
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-freg-struct-return`: not supported for target
// SLATE-FILECHECK-END PARSE
