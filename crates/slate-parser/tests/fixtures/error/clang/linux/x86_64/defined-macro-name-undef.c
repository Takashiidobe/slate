// SLATE-FILECHECK-ERROR UNDEF

#undef defined
int x;

// SLATE-FILECHECK-BEGIN UNDEF
// UNDEF: Error:   × 'defined' cannot be used as a macro name
// UNDEF:   ╰─▶ 'defined' cannot be used as a macro name
// UNDEF:    ╭─[tests/fixtures/error/clang/linux/x86_64/defined-macro-name-undef.c:2:8]
// UNDEF:  1 │
// UNDEF:  2 │ #undef defined
// UNDEF:    ·        ───────
// UNDEF:  3 │ int x;
// UNDEF:    ╰────
// SLATE-FILECHECK-END UNDEF
