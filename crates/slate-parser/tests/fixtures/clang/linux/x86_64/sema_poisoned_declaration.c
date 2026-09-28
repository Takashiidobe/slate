struct S { int a; } s;
int bad(void) { return s + 1; }
int calls_bad(void) { return bad(); }
int recursive(void) { return recursive() + s; }

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: non-arithmetic operand
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/sema_poisoned_declaration.c:2:24]
// DEFAULT: 1 │ struct S { int a; } s;
// DEFAULT: 2 │ int bad(void) { return s + 1; }
// DEFAULT: ·                        ─────
// DEFAULT: 3 │ int calls_bad(void) { return bad(); }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: non-arithmetic operand
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/sema_poisoned_declaration.c:4:30]
// DEFAULT: 3 │ int calls_bad(void) { return bad(); }
// DEFAULT: 4 │ int recursive(void) { return recursive() + s; }
// DEFAULT: ·                              ───────────────
// DEFAULT: 5 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
