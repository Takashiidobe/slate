int value;

// SLATE-FILECHECK-ARGS -fopenmp -fopenmp-version=45
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fopenmp-version=45`: unknown option for the
// SLATE-FILECHECK-END PARSE
