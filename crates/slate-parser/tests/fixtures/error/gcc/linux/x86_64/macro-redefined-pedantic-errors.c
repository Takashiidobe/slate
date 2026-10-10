// SLATE-FILECHECK-ARGS -pedantic-errors
// SLATE-FILECHECK-ERROR PARSE

#define CHANGED 1
#define CHANGED 2
int x;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error: -Wmacro-redefined
// PARSE: × 'CHANGED' redefined
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/macro-redefined-pedantic-errors.c:3:9]
// PARSE: 2 │ #define CHANGED 1
// PARSE: 3 │ #define CHANGED 2
// PARSE: ·         ───────
// PARSE: 4 │ int x;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
