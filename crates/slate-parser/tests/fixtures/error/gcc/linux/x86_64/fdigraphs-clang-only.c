// SLATE-FILECHECK-ARGS -fdigraphs
// SLATE-FILECHECK-ERROR PARSE

int value;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fdigraphs`: unknown option for the gcc flavor
// SLATE-FILECHECK-END PARSE
