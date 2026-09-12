#warning heads up
#ifdef SLATE_FORBID
#error SLATE_FORBID must not be defined
#endif
int value;

// SLATE-FILECHECK-DEFINES FORBID SLATE_FORBID
// SLATE-FILECHECK-ERROR FORBID

// SLATE-FILECHECK-BEGIN FORBID
// FORBID: ⚠ #warning heads up
// FORBID: ╭─[tests/fixtures/pp-directive-forbid.c:1:1]
// FORBID: 1 │ #warning heads up
// FORBID: · ─────────────────
// FORBID: 2 │ #ifdef SLATE_FORBID
// FORBID: ╰────
// FORBID: Error:   × #error SLATE_FORBID must not be defined
// FORBID: ╭─[tests/fixtures/pp-directive-forbid.c:3:1]
// FORBID: 2 │ #ifdef SLATE_FORBID
// FORBID: 3 │ #error SLATE_FORBID must not be defined
// FORBID: · ───────────────────────────────────────
// FORBID: 4 │ #endif
// FORBID: ╰────
// SLATE-FILECHECK-END FORBID
