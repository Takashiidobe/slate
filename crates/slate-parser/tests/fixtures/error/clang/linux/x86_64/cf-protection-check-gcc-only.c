int value;

// SLATE-FILECHECK-ARGS -fcf-protection=check
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: all rules failed: Clang control-flow protection: `check`
// SLATE-FILECHECK-END PARSE
