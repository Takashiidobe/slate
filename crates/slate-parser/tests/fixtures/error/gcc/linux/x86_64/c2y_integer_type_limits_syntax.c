void invalid(void) {
#ifdef EMPTY
    _Maxof();
#elif defined(EXPR)
    _Minof(1 + 2);
#elif defined(BARE)
    _Maxof int;
#elif defined(EXTRA)
    _Minof(int, long);
#elif defined(CLOSE)
    _Maxof(int;
#else
    int x = 0;
    _Minof(x);
#endif
}

// SLATE-FILECHECK-STD EMPTY c2y
// SLATE-FILECHECK-DEFINES EMPTY EMPTY
// SLATE-FILECHECK-ERROR EMPTY
// SLATE-FILECHECK-STD EXPR c2y
// SLATE-FILECHECK-DEFINES EXPR EXPR
// SLATE-FILECHECK-ERROR EXPR
// SLATE-FILECHECK-STD BARE c2y
// SLATE-FILECHECK-DEFINES BARE BARE
// SLATE-FILECHECK-ERROR BARE
// SLATE-FILECHECK-STD EXTRA c2y
// SLATE-FILECHECK-DEFINES EXTRA EXTRA
// SLATE-FILECHECK-ERROR EXTRA
// SLATE-FILECHECK-STD CLOSE c2y
// SLATE-FILECHECK-DEFINES CLOSE CLOSE
// SLATE-FILECHECK-ERROR CLOSE
// SLATE-FILECHECK-STD VALUE c2y
// SLATE-FILECHECK-DEFINES VALUE
// SLATE-FILECHECK-ERROR VALUE

// SLATE-FILECHECK-BEGIN EMPTY
// EMPTY: Error:   × expected type name
// EMPTY: ╰─▶ expected type name
// EMPTY: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_integer_type_limits_syntax.c:3:5]
// EMPTY: 2 │ #ifdef EMPTY
// EMPTY: 3 │     _Maxof();
// EMPTY: ·     ──────
// EMPTY: 4 │ #elif defined(EXPR)
// EMPTY: ╰────
// SLATE-FILECHECK-END EMPTY
// SLATE-FILECHECK-BEGIN EXPR
// EXPR: Error:   × expected type name
// EXPR: ╰─▶ expected type name
// EXPR: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_integer_type_limits_syntax.c:5:5]
// EXPR: 4 │ #elif defined(EXPR)
// EXPR: 5 │     _Minof(1 + 2);
// EXPR: ·     ──────
// EXPR: 6 │ #elif defined(BARE)
// EXPR: ╰────
// SLATE-FILECHECK-END EXPR
// SLATE-FILECHECK-BEGIN BARE
// BARE: Error:   × expected `(`, found `int`
// BARE: ╰─▶ expected `(`, found `int`
// BARE: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_integer_type_limits_syntax.c:7:5]
// BARE: 6 │ #elif defined(BARE)
// BARE: 7 │     _Maxof int;
// BARE: ·     ──────
// BARE: 8 │ #elif defined(EXTRA)
// BARE: ╰────
// SLATE-FILECHECK-END BARE
// SLATE-FILECHECK-BEGIN EXTRA
// EXTRA: Error:   × expected `)`, found `,`
// EXTRA: ╰─▶ expected `)`, found `,`
// EXTRA: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_integer_type_limits_syntax.c:9:5]
// EXTRA: 8 │ #elif defined(EXTRA)
// EXTRA: 9 │     _Minof(int, long);
// EXTRA: ·     ──────
// EXTRA: 10 │ #elif defined(CLOSE)
// EXTRA: ╰────
// SLATE-FILECHECK-END EXTRA
// SLATE-FILECHECK-BEGIN CLOSE
// CLOSE: Error:   × expected `;`
// CLOSE: ╰─▶ expected `;`
// CLOSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_integer_type_limits_syntax.c:11:5]
// CLOSE: 10 │ #elif defined(CLOSE)
// CLOSE: 11 │     _Maxof(int;
// CLOSE: ·     ──────
// CLOSE: 12 │ #else
// CLOSE: ╰────
// SLATE-FILECHECK-END CLOSE
// SLATE-FILECHECK-BEGIN VALUE
// VALUE: Error:   × expected type name
// VALUE: ╰─▶ expected type name
// VALUE: ╭─[tests/fixtures/error/gcc/linux/x86_64/c2y_integer_type_limits_syntax.c:14:5]
// VALUE: 13 │     int x = 0;
// VALUE: 14 │     _Minof(x);
// VALUE: ·     ──────
// VALUE: 15 │ #endif
// VALUE: ╰────
// SLATE-FILECHECK-END VALUE
