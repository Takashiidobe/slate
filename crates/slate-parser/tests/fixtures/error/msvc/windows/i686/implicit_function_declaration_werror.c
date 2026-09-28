void f(void) { g(4); }

// SLATE-FILECHECK-ARGS --dump-ir -Werror=implicit-function-declaration
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error: -Wimplicit-function-declaration
// DEFAULT: × implicit declaration of function 'g'; assuming extern returning int
// DEFAULT: ╭─[tests/fixtures/error/msvc/windows/i686/implicit_function_declaration_werror.c:1:16]
// DEFAULT: 1 │ void f(void) { g(4); }
// DEFAULT: ·                ─
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
