int value;

// SLATE-FILECHECK-ARGS -fpack-struct=2
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fpack-struct=2`: unknown option for the msvc
// SLATE-FILECHECK-END PARSE
