int a = undeclared, b;
int uses_b(void) { return b + p + q; }
int jumps(void) { goto missing; }
int calls_failed(void) { return uses_b(); }
int redeclares(void) { int x; int x; return 0; }

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `undeclared`
// DEFAULT: ╭─[tests/fixtures/sema_reports_all_errors.c:1:9]
// DEFAULT: 1 │ int a = undeclared, b;
// DEFAULT: ·         ──────────
// DEFAULT: 2 │ int uses_b(void) { return b + p + q; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `p`
// DEFAULT: ╭─[tests/fixtures/sema_reports_all_errors.c:2:31]
// DEFAULT: 1 │ int a = undeclared, b;
// DEFAULT: 2 │ int uses_b(void) { return b + p + q; }
// DEFAULT: ·                               ─
// DEFAULT: 3 │ int jumps(void) { goto missing; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `q`
// DEFAULT: ╭─[tests/fixtures/sema_reports_all_errors.c:2:35]
// DEFAULT: 1 │ int a = undeclared, b;
// DEFAULT: 2 │ int uses_b(void) { return b + p + q; }
// DEFAULT: ·                                   ─
// DEFAULT: 3 │ int jumps(void) { goto missing; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved label name `missing`
// DEFAULT: ╭─[tests/fixtures/sema_reports_all_errors.c:3:24]
// DEFAULT: 2 │ int uses_b(void) { return b + p + q; }
// DEFAULT: 3 │ int jumps(void) { goto missing; }
// DEFAULT: ·                        ───────
// DEFAULT: 4 │ int calls_failed(void) { return uses_b(); }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × duplicate ordinary name `x`
// DEFAULT: ╭─[tests/fixtures/sema_reports_all_errors.c:5:35]
// DEFAULT: 4 │ int calls_failed(void) { return uses_b(); }
// DEFAULT: 5 │ int redeclares(void) { int x; int x; return 0; }
// DEFAULT: ·                                   ─
// DEFAULT: 6 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
