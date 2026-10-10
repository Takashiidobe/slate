// SLATE-FILECHECK-ARGS -Werror=builtin-macro-redefined
// SLATE-FILECHECK-ERROR PARSE

#undef __STDC_VERSION__
int x;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error: -Wbuiltin-macro-redefined
// PARSE: × undefining builtin macro
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/builtin-macro-redefined-werror.c:2:8]
// PARSE: 1 │
// PARSE: 2 │ #undef __STDC_VERSION__
// PARSE: ·        ────────────────
// PARSE: 3 │ int x;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
