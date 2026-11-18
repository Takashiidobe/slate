#warning hi
int x;

// SLATE-FILECHECK-ARGS -Werror
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × #warning hi
// DEFAULT:    ╭─[tests/fixtures/error/clang/linux/x86_64/warning_directive_werror.c:1:1]
// DEFAULT:  1 │ #warning hi
// DEFAULT:    · ───────────
// DEFAULT:  2 │ int x;
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
