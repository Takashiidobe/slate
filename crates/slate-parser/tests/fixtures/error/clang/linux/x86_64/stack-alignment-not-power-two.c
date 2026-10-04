int value;

// SLATE-FILECHECK-ARGS -mstack-alignment=6
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: Clang stack alignment: expected a power of two, found 6
// SLATE-FILECHECK-END PARSE
