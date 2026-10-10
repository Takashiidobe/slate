// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-ERROR C11

#define A

#if 1
int taken;
#elifdef A
#endif

// SLATE-FILECHECK-BEGIN C11
// C11: Error:   × unsupported preprocessor directive
// C11: ╰─▶ unsupported preprocessor directive
// C11: ╭─[tests/fixtures/error/gcc/linux/x86_64/elifdef-c11.c:6:2]
// C11: 5 │ int taken;
// C11: 6 │ #elifdef A
// C11: ·  ───────
// C11: 7 │ #endif
// C11: ╰────
// SLATE-FILECHECK-END C11
