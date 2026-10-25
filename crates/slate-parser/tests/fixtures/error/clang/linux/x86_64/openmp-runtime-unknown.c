int value;

// SLATE-FILECHECK-ARGS -fopenmp=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fopenmp=bogus`: unsupported OpenMP runtime;
// SLATE-FILECHECK-END PARSE
