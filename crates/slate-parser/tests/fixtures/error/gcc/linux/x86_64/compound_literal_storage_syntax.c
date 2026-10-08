void invalid(void) {
#if defined(TRAILING)
    (int register){1};
#elif defined(TYPE)
    (register){1};
#elif defined(DUPLICATE)
    (static register int){1};
#elif defined(CAST)
    (static int)1;
#elif defined(SIZEOF)
    sizeof(static int);
#else
    (register int[2]){1, 2;
#endif
}

// SLATE-FILECHECK-DEFINES TRAILING TRAILING
// SLATE-FILECHECK-STD TRAILING c17
// SLATE-FILECHECK-ERROR TRAILING
// SLATE-FILECHECK-DEFINES TYPE TYPE
// SLATE-FILECHECK-STD TYPE c23
// SLATE-FILECHECK-ERROR TYPE
// SLATE-FILECHECK-DEFINES DUPLICATE DUPLICATE
// SLATE-FILECHECK-STD DUPLICATE c23
// SLATE-FILECHECK-ERROR DUPLICATE
// SLATE-FILECHECK-DEFINES CAST CAST
// SLATE-FILECHECK-STD CAST c23
// SLATE-FILECHECK-ERROR CAST
// SLATE-FILECHECK-DEFINES SIZEOF SIZEOF
// SLATE-FILECHECK-STD SIZEOF c23
// SLATE-FILECHECK-ERROR SIZEOF
// SLATE-FILECHECK-DEFINES BRACE
// SLATE-FILECHECK-STD BRACE c23
// SLATE-FILECHECK-ERROR BRACE

// SLATE-FILECHECK-BEGIN TRAILING
// TRAILING: Error:   × storage-class specifiers in compound literals require C23
// TRAILING: ╰─▶ storage-class specifiers in compound literals require C23
// TRAILING: ╭─[tests/fixtures/error/gcc/linux/x86_64/compound_literal_storage_syntax.c:3:5]
// TRAILING: 2 │ #if defined(TRAILING)
// TRAILING: 3 │     (int register){1};
// TRAILING: ·     ─
// TRAILING: 4 │ #elif defined(TYPE)
// TRAILING: ╰────
// SLATE-FILECHECK-END TRAILING
// SLATE-FILECHECK-BEGIN TYPE
// TYPE: Error:   × unexpected tokens after expression
// TYPE: ╰─▶ unexpected tokens after expression
// TYPE: ╭─[tests/fixtures/error/gcc/linux/x86_64/compound_literal_storage_syntax.c:5:5]
// TYPE: 4 │ #elif defined(TYPE)
// TYPE: 5 │     (register){1};
// TYPE: ·     ─
// TYPE: 6 │ #elif defined(DUPLICATE)
// TYPE: ╰────
// SLATE-FILECHECK-END TYPE
// SLATE-FILECHECK-BEGIN DUPLICATE
// DUPLICATE: Error:   × expected `)`, found `register`
// DUPLICATE: ╰─▶ expected `)`, found `register`
// DUPLICATE: ╭─[tests/fixtures/error/gcc/linux/x86_64/compound_literal_storage_syntax.c:7:5]
// DUPLICATE: 6 │ #elif defined(DUPLICATE)
// DUPLICATE: 7 │     (static register int){1};
// DUPLICATE: ·     ─
// DUPLICATE: 8 │ #elif defined(CAST)
// DUPLICATE: ╰────
// SLATE-FILECHECK-END DUPLICATE
// SLATE-FILECHECK-BEGIN CAST
// CAST: Error:   × expected `)`, found `int`
// CAST: ╰─▶ expected `)`, found `int`
// CAST: ╭─[tests/fixtures/error/gcc/linux/x86_64/compound_literal_storage_syntax.c:9:5]
// CAST: 8 │ #elif defined(CAST)
// CAST: 9 │     (static int)1;
// CAST: ·     ─
// CAST: 10 │ #elif defined(SIZEOF)
// CAST: ╰────
// SLATE-FILECHECK-END CAST
// SLATE-FILECHECK-BEGIN SIZEOF
// SIZEOF: Error:   × expected `)`, found `int`
// SIZEOF: ╰─▶ expected `)`, found `int`
// SIZEOF: ╭─[tests/fixtures/error/gcc/linux/x86_64/compound_literal_storage_syntax.c:11:5]
// SIZEOF: 10 │ #elif defined(SIZEOF)
// SIZEOF: 11 │     sizeof(static int);
// SIZEOF: ·     ──────
// SIZEOF: 12 │ #else
// SIZEOF: ╰────
// SLATE-FILECHECK-END SIZEOF
// SLATE-FILECHECK-BEGIN BRACE
// BRACE: Error:   × expected `}`
// BRACE: ╰─▶ expected `}`
// BRACE: ╭─[tests/fixtures/error/gcc/linux/x86_64/compound_literal_storage_syntax.c:1:20]
// BRACE: 1 │ void invalid(void) {
// BRACE: ·                    ─
// BRACE: 2 │ #if defined(TRAILING)
// BRACE: ╰────
// SLATE-FILECHECK-END BRACE
