int value;

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fopenmp`: unknown option for the msvc flavor
// SLATE-FILECHECK-END PARSE
