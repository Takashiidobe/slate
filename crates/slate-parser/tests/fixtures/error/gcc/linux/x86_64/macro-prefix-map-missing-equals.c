int value;

// SLATE-FILECHECK-ARGS -fmacro-prefix-map=a/b/t.c
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-fmacro-prefix-map=a/b/t.c`: expected OLD=NEW
// SLATE-FILECHECK-END PARSE
