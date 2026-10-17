int value;

// SLATE-FILECHECK-ARGS -finput-charset=latin1
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-finput-charset=latin1`: expected one of utf-8,
// SLATE-FILECHECK-END PARSE
