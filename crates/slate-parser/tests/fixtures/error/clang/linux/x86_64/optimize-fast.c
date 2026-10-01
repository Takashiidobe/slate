int value;

// SLATE-FILECHECK-ARGS -Ofast
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-Ofast`: fast-math is not emulated
// SLATE-FILECHECK-END PARSE
