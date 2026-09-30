// SLATE-FILECHECK-ARGS -masm=intel
// SLATE-FILECHECK-ERROR PARSE

int value;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: MSVC stack alignment options: MSVC does not support
// SLATE-FILECHECK-END PARSE
