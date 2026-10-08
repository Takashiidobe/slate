void invalid(void) {
loop:
    while (1) {
#ifdef EXTRA
        continue loop extra;
#else
        continue loop
#endif
    }
}

// SLATE-FILECHECK-STD MISSING c2y
// SLATE-FILECHECK-ERROR MISSING
// SLATE-FILECHECK-STD EXTRA c2y
// SLATE-FILECHECK-ERROR EXTRA
// SLATE-FILECHECK-DEFINES EXTRA EXTRA
// SLATE-FILECHECK-DEFINES MISSING

// SLATE-FILECHECK-BEGIN MISSING
// MISSING: Error:   × expected `;`
// MISSING: ╰─▶ expected `;`
// MISSING: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_named_continue_syntax.c:7:18]
// MISSING: 6 │ #else
// MISSING: 7 │         continue loop
// MISSING: ·                  ────
// MISSING: 8 │ #endif
// MISSING: ╰────
// SLATE-FILECHECK-END MISSING
// SLATE-FILECHECK-BEGIN EXTRA
// EXTRA: Error:   × expected `;`
// EXTRA: ╰─▶ expected `;`
// EXTRA: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_named_continue_syntax.c:5:23]
// EXTRA: 4 │ #ifdef EXTRA
// EXTRA: 5 │         continue loop extra;
// EXTRA: ·                       ─────
// EXTRA: 6 │ #else
// EXTRA: ╰────
// SLATE-FILECHECK-END EXTRA
