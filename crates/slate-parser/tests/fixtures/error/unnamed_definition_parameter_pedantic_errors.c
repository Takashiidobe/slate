int f(int) { return 0; }

// SLATE-FILECHECK-ARGS -pedantic-errors
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ERROR C17

// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × semantic analysis failed
// C17: Error: -Wc23-extensions
// C17: × omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/error/unnamed_definition_parameter_pedantic_errors.c:1:7]
// C17: 1 │ int f(int) { return 0; }
// C17: ·       ───
// C17: 2 │
// C17: ╰────
// SLATE-FILECHECK-END C17
