int value;

// SLATE-FILECHECK-ARGS -ffp-contract=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ffp-contract=bogus`: expected one of off, on,
// SLATE-FILECHECK-END PARSE
