static x;
f() { return 1; }

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ERROR C23

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × a type specifier is required for all declarations
// C23: ╰─▶ a type specifier is required for all declarations
// C23: ╭─[tests/fixtures/implicit_int_by_standard.c:1:8]
// C23: 1 │ static x;
// C23: ·        ─
// C23: 2 │ f() { return 1; }
// C23: ╰────
// SLATE-FILECHECK-END C23
