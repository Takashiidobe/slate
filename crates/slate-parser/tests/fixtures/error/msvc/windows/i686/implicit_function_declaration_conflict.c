void f(void) { g(4); }
void g(void) {}

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × conflicting types for function redeclaration
// DEFAULT: ╭─[tests/fixtures/error/msvc/windows/i686/implicit_function_declaration_conflict.c:2:1]
// DEFAULT: 1 │ void f(void) { g(4); }
// DEFAULT: 2 │ void g(void) {}
// DEFAULT: · ───────────────
// DEFAULT: 3 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
