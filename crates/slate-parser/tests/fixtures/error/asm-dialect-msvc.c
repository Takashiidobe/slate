// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc -masm=intel
// SLATE-FILECHECK-ERROR PARSE

int value;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: MSVC stack alignment options: MSVC does not support
// SLATE-FILECHECK-END PARSE
