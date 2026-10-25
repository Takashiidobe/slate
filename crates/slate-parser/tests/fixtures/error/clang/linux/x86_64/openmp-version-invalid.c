int value;

// SLATE-FILECHECK-ARGS -fopenmp -fopenmp-version=abc
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fopenmp-version=abc`: invalid integral value
// SLATE-FILECHECK-END PARSE
