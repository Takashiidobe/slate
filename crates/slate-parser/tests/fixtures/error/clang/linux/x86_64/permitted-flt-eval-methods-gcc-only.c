int value;

// SLATE-FILECHECK-ARGS -fpermitted-flt-eval-methods=c11
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fpermitted-flt-eval-methods=c11`: unknown
// SLATE-FILECHECK-END PARSE
