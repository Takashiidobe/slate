#ifdef FIRST
int first;
#else
int other;
#elifdef SECOND
int second;
#endif

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #elifdef after #else
// PARSE: ╰─▶ #elifdef after #else
// PARSE: ╭─[tests/fixtures/error/elifdef-after-else.c:5:1]
// PARSE: 4 │ int other;
// PARSE: 5 │ #elifdef SECOND
// PARSE: · ───────────────
// PARSE: 6 │ int second;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
