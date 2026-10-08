void rejected(void) {
#ifdef SWITCH
    switch (int x = 1; x) { default: break; }
#else
    if (int x = 1) {}
#endif
}

// SLATE-FILECHECK-DEFINES IF
// SLATE-FILECHECK-STD IF c2y
// SLATE-FILECHECK-ERROR IF
// SLATE-FILECHECK-DEFINES SWITCH SWITCH
// SLATE-FILECHECK-STD SWITCH gnu2y
// SLATE-FILECHECK-ERROR SWITCH

// SLATE-FILECHECK-BEGIN IF
// IF: Error:   × selection statement declarations require C2y in the GCC flavor
// IF: ╰─▶ selection statement declarations require C2y in the GCC flavor
// IF: ╭─[tests/fixtures/error/clang/linux/x86_64/c2y_selection_declarations.c:5:9]
// IF: 4 │ #else
// IF: 5 │     if (int x = 1) {}
// IF: ·         ───
// IF: 6 │ #endif
// IF: ╰────
// SLATE-FILECHECK-END IF
// SLATE-FILECHECK-BEGIN SWITCH
// SWITCH: Error:   × selection statement declarations require C2y in the GCC flavor
// SWITCH: ╰─▶ selection statement declarations require C2y in the GCC flavor
// SWITCH: ╭─[tests/fixtures/error/clang/linux/x86_64/c2y_selection_declarations.c:3:13]
// SWITCH: 2 │ #ifdef SWITCH
// SWITCH: 3 │     switch (int x = 1; x) { default: break; }
// SWITCH: ·             ───
// SWITCH: 4 │ #else
// SWITCH: ╰────
// SLATE-FILECHECK-END SWITCH
