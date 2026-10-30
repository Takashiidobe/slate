int value;

// SLATE-FILECHECK-ARGS -mno-sse -mfpmath=sse
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × all rules failed: target ISA options: the 'sse' unit is not supported with
// SLATE-FILECHECK-END PARSE
