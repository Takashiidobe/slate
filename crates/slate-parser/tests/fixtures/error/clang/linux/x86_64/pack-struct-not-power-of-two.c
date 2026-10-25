int value;

// SLATE-FILECHECK-ARGS -fpack-struct=3
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: all rules failed: Clang structure packing: a structure
// SLATE-FILECHECK-END PARSE
