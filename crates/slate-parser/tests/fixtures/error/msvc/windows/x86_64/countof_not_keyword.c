// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

int a[3];
int b[_Countof (a)];

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × variable length array with static storage duration
// SEMANTIC: ╭─[tests/fixtures/error/msvc/windows/x86_64/countof_not_keyword.c:3:5]
// SEMANTIC: 2 │ int a[3];
// SEMANTIC: 3 │ int b[_Countof (a)];
// SEMANTIC: ·     ───────────────
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
