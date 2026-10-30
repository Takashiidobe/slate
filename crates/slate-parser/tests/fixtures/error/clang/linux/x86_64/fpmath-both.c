int value;

// SLATE-FILECHECK-ARGS -mfpmath=both
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: target ISA options: unknown FP unit for x86_64-unknown-
// SLATE-FILECHECK-END PARSE
