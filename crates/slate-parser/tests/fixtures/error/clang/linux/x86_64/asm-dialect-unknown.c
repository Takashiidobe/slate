int value;

// SLATE-FILECHECK-ARGS -masm=bogus
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-masm=bogus`: invalid asm dialect: unknown asm
// SLATE-FILECHECK-END PARSE
