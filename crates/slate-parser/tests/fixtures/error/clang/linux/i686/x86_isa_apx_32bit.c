int x;

// SLATE-FILECHECK-ARGS -mapxf
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: target ISA options: -mapxf is not supported for 32-bit
// SLATE-FILECHECK-END PARSE
