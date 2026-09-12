#if 0
#ifdef NESTED
#else
#else
#endif
#endif
int value;

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × multiple #else directives
// PARSE: ╰─▶ multiple #else directives
// PARSE: ╭─[tests/fixtures/dead-branch-malformed-conditional.c:4:1]
// PARSE: 3 │ #else
// PARSE: 4 │ #else
// PARSE: · ─────
// PARSE: 5 │ #endif
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
