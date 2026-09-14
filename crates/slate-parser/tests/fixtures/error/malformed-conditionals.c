#if 1
int value;
#else
#elif 0
#endif

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #elif after #else
// PARSE: ╰─▶ #elif after #else
// PARSE: ╭─[tests/fixtures/error/malformed-conditionals.c:4:1]
// PARSE: 3 │ #else
// PARSE: 4 │ #elif 0
// PARSE: · ───────
// PARSE: 5 │ #endif
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
