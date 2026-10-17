int value;

// SLATE-FILECHECK-ARGS -fpermitted-flt-eval-methods=c99
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fpermitted-flt-eval-methods=c99`: expected one
// SLATE-FILECHECK-END PARSE
