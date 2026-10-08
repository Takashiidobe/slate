void invalid(void) {
loop:
    while (1) {
#ifdef EXTRA
        break loop extra;
#else
        break loop
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
// MISSING: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_named_break_syntax.c:7:15]
// MISSING: 6 │ #else
// MISSING: 7 │         break loop
// MISSING: ·               ────
// MISSING: 8 │ #endif
// MISSING: ╰────
// SLATE-FILECHECK-END MISSING
// SLATE-FILECHECK-BEGIN EXTRA
// EXTRA: Error:   × expected `;`
// EXTRA: ╰─▶ expected `;`
// EXTRA: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_named_break_syntax.c:5:20]
// EXTRA: 4 │ #ifdef EXTRA
// EXTRA: 5 │         break loop extra;
// EXTRA: ·                    ─────
// EXTRA: 6 │ #else
// EXTRA: ╰────
// SLATE-FILECHECK-END EXTRA
