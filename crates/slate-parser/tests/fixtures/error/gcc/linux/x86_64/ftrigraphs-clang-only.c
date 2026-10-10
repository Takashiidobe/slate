int value;

// SLATE-FILECHECK-ARGS -ftrigraphs
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ftrigraphs`: unknown option for the gcc flavor
// SLATE-FILECHECK-END PARSE
