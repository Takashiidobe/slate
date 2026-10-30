int value;

// SLATE-FILECHECK-ARGS -mfpmath=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-mfpmath=bogus`: invalid FP unit: unknown FP
// SLATE-FILECHECK-END PARSE
