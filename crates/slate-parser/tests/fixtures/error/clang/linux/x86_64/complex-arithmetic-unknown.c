int value;

// SLATE-FILECHECK-ARGS -fcomplex-arithmetic=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fcomplex-arithmetic=bogus`: expected one of
// SLATE-FILECHECK-END PARSE
