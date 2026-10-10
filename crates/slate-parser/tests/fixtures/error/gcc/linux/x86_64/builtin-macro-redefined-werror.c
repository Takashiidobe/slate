// SLATE-FILECHECK-ARGS -Werror=builtin-macro-redefined
// SLATE-FILECHECK-ERROR PARSE

#define CHANGED 1
#define CHANGED 2
#define __TIME__ "now"
int x;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: ⚠ 'CHANGED' redefined
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/builtin-macro-redefined-werror.c:3:9]
// PARSE: 2 │ #define CHANGED 1
// PARSE: 3 │ #define CHANGED 2
// PARSE: ·         ───────
// PARSE: 4 │ #define __TIME__ "now"
// PARSE: ╰────
// PARSE: Error: -Wbuiltin-macro-redefined
// PARSE: × '__TIME__' redefined
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/builtin-macro-redefined-werror.c:4:9]
// PARSE: 3 │ #define CHANGED 2
// PARSE: 4 │ #define __TIME__ "now"
// PARSE: ·         ────────
// PARSE: 5 │ int x;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
