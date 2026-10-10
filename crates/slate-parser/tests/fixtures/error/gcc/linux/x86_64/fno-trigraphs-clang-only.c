int value;

// SLATE-FILECHECK-ARGS -fno-trigraphs
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fno-trigraphs`: unknown option for the gcc
// SLATE-FILECHECK-END PARSE
