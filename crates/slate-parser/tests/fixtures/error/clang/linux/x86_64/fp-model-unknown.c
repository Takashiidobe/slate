int value;

// SLATE-FILECHECK-ARGS -ffp-model=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ffp-model=bogus`: expected one of precise,
// SLATE-FILECHECK-END PARSE
