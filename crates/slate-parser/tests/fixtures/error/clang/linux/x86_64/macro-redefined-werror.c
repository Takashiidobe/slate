// SLATE-FILECHECK-ARGS -Werror=macro-redefined
// SLATE-FILECHECK-ERROR PARSE

#define SAME 1
#define SAME 1
#define CHANGED 1
#define CHANGED 2
int x;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error: -Wmacro-redefined
// PARSE: × 'CHANGED' macro redefined
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/macro-redefined-werror.c:5:9]
// PARSE: 4 │ #define CHANGED 1
// PARSE: 5 │ #define CHANGED 2
// PARSE: ·         ───────
// PARSE: 6 │ int x;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
