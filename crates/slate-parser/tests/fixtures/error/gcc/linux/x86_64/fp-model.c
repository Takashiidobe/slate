int value;

// SLATE-FILECHECK-ARGS -ffp-model=fast
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-ffp-model=fast`: unknown option for the gcc
// SLATE-FILECHECK-END PARSE
