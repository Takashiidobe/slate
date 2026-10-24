// SLATE-FILECHECK-ERROR DEFINE

#define defined 1
int x;

// SLATE-FILECHECK-BEGIN DEFINE
// DEFINE: Error:   × 'defined' cannot be used as a macro name
// DEFINE:   ╰─▶ 'defined' cannot be used as a macro name
// DEFINE:    ╭─[tests/fixtures/error/clang/linux/x86_64/defined-macro-name-define.c:2:9]
// DEFINE:  1 │
// DEFINE:  2 │ #define defined 1
// DEFINE:    ·         ───────
// DEFINE:  3 │ int x;
// DEFINE:    ╰────
// SLATE-FILECHECK-END DEFINE
