// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ERROR C17

#define A

#if 1
int taken;
#elifdef A
#endif

// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × unsupported preprocessor directive
// C17: ╰─▶ unsupported preprocessor directive
// C17: ╭─[tests/fixtures/error/msvc/windows/x86_64/elifdef-c17.c:6:2]
// C17: 5 │ int taken;
// C17: 6 │ #elifdef A
// C17: ·  ───────
// C17: 7 │ #endif
// C17: ╰────
// SLATE-FILECHECK-END C17
