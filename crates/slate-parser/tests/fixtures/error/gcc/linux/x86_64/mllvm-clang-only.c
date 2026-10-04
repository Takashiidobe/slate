int value;

// SLATE-FILECHECK-ARGS -mllvm -x86-asm-syntax=att
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-mllvm`: unknown option for the gcc flavor
// SLATE-FILECHECK-END PARSE
