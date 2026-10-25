int value;

// SLATE-FILECHECK-ARGS -fno-inline
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fno-inline`: unknown option for the msvc
// SLATE-FILECHECK-END PARSE
