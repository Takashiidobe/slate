int first(int, int b) { return b; }
void pointer(char *, long) {}
int named(int a) { return a; }
int declaration_only(int, int);

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C89
// SLATE-FILECHECK-WARNING C17

// SLATE-FILECHECK-BEGIN C89
// C89: -Wc23-extensions
// C89: ⚠ omitting the parameter name in a function definition is a C23 extension
// C89: ╭─[tests/fixtures/sema/unnamed_definition_parameter_warnings.c:1:11]
// C89: 1 │ int first(int, int b) { return b; }
// C89: ·           ───
// C89: 2 │ void pointer(char *, long) {}
// C89: ╰────
// C89: -Wc23-extensions
// C89: ⚠ omitting the parameter name in a function definition is a C23 extension
// C89: ╭─[tests/fixtures/sema/unnamed_definition_parameter_warnings.c:2:14]
// C89: 1 │ int first(int, int b) { return b; }
// C89: 2 │ void pointer(char *, long) {}
// C89: ·              ──────
// C89: 3 │ int named(int a) { return a; }
// C89: ╰────
// C89: -Wc23-extensions
// C89: ⚠ omitting the parameter name in a function definition is a C23 extension
// C89: ╭─[tests/fixtures/sema/unnamed_definition_parameter_warnings.c:2:22]
// C89: 1 │ int first(int, int b) { return b; }
// C89: 2 │ void pointer(char *, long) {}
// C89: ·                      ────
// C89: 3 │ int named(int a) { return a; }
// C89: ╰────
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C17
// C17: -Wc23-extensions
// C17: ⚠ omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/sema/unnamed_definition_parameter_warnings.c:1:11]
// C17: 1 │ int first(int, int b) { return b; }
// C17: ·           ───
// C17: 2 │ void pointer(char *, long) {}
// C17: ╰────
// C17: -Wc23-extensions
// C17: ⚠ omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/sema/unnamed_definition_parameter_warnings.c:2:14]
// C17: 1 │ int first(int, int b) { return b; }
// C17: 2 │ void pointer(char *, long) {}
// C17: ·              ──────
// C17: 3 │ int named(int a) { return a; }
// C17: ╰────
// C17: -Wc23-extensions
// C17: ⚠ omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/sema/unnamed_definition_parameter_warnings.c:2:22]
// C17: 1 │ int first(int, int b) { return b; }
// C17: 2 │ void pointer(char *, long) {}
// C17: ·                      ────
// C17: 3 │ int named(int a) { return a; }
// C17: ╰────
// SLATE-FILECHECK-END C17
