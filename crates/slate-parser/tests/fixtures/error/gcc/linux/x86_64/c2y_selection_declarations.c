void invalid(void) {
#ifdef SWITCH
    switch (int x = 1;) { default: break; }
#else
    if (int x = 1;) {}
#endif
}

// SLATE-FILECHECK-DEFINES IF
// SLATE-FILECHECK-STD IF c2y
// SLATE-FILECHECK-ERROR IF
// SLATE-FILECHECK-DEFINES SWITCH SWITCH
// SLATE-FILECHECK-STD SWITCH c2y
// SLATE-FILECHECK-ERROR SWITCH

// SLATE-FILECHECK-BEGIN IF
// IF: Error:   × expected expression after selection declaration
// IF: ╰─▶ expected expression after selection declaration
// IF: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_selection_declarations.c:5:18]
// IF: 4 │ #else
// IF: 5 │     if (int x = 1;) {}
// IF: ·                  ─
// IF: 6 │ #endif
// IF: ╰────
// SLATE-FILECHECK-END IF
// SLATE-FILECHECK-BEGIN SWITCH
// SWITCH: Error:   × expected expression after selection declaration
// SWITCH: ╰─▶ expected expression after selection declaration
// SWITCH: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_selection_declarations.c:3:22]
// SWITCH: 2 │ #ifdef SWITCH
// SWITCH: 3 │     switch (int x = 1;) { default: break; }
// SWITCH: ·                      ─
// SWITCH: 4 │ #else
// SWITCH: ╰────
// SLATE-FILECHECK-END SWITCH
