int value;

// SLATE-FILECHECK-ARGS -ffp-contract=off
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ffp-contract=off`: unknown option for the msvc
// SLATE-FILECHECK-END PARSE
