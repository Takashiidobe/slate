// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 gnu17
// SLATE-FILECHECK-ERROR C99
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 gnu23
// SLATE-FILECHECK-PREFIX-ARGS C23 -Wno-implicit
// SLATE-FILECHECK-ERROR C23
// SLATE-FILECHECK-DEFINES BUILTIN BUILTIN
// SLATE-FILECHECK-STD BUILTIN c89
// SLATE-FILECHECK-ERROR BUILTIN
// SLATE-FILECHECK-DEFINES WERROR_INT WERROR_INT
// SLATE-FILECHECK-STD WERROR_INT c89
// SLATE-FILECHECK-PREFIX-ARGS WERROR_INT -Werror=implicit-int
// SLATE-FILECHECK-ERROR WERROR_INT
// SLATE-FILECHECK-DEFINES IMPLICIT_INT IMPLICIT_INT
// SLATE-FILECHECK-STD IMPLICIT_INT gnu17
// SLATE-FILECHECK-ERROR IMPLICIT_INT
// SLATE-FILECHECK-ARGS --dump-ir

#if defined(WERROR_INT) || defined(IMPLICIT_INT)
static counter;
#elif defined(BUILTIN)
int f(void) { return __builtin_not_a_builtin(1); }
#else
int f(void) { return undeclared(1); }
#endif

// SLATE-FILECHECK-BEGIN C99
// C99: Error:   × semantic analysis failed
// C99: Error: -Wimplicit-function-declaration
// C99:   × call to undeclared function 'undeclared'; ISO C99 and later do not support
// C99:    ╭─[tests/fixtures/error/clang/linux/x86_64/implicit_function_modes.c:7:22]
// C99:  6 │ #else
// C99:  7 │ int f(void) { return undeclared(1); }
// C99:    ·                      ──────────
// C99:  8 │ #endif
// C99:    ╰────
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × semantic analysis failed
// C23: Error:
// C23:   × unresolved ordinary name `undeclared`
// C23:    ╭─[tests/fixtures/error/clang/linux/x86_64/implicit_function_modes.c:7:22]
// C23:  6 │ #else
// C23:  7 │ int f(void) { return undeclared(1); }
// C23:    ·                      ──────────
// C23:  8 │ #endif
// C23:    ╰────
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN BUILTIN
// BUILTIN: Error:   × semantic analysis failed
// BUILTIN: Error: -Wimplicit-function-declaration
// BUILTIN:   × use of unknown builtin '__builtin_not_a_builtin'
// BUILTIN:    ╭─[tests/fixtures/error/clang/linux/x86_64/implicit_function_modes.c:5:22]
// BUILTIN:  4 │ #elif defined(BUILTIN)
// BUILTIN:  5 │ int f(void) { return __builtin_not_a_builtin(1); }
// BUILTIN:    ·                      ───────────────────────
// BUILTIN:  6 │ #else
// BUILTIN:    ╰────
// SLATE-FILECHECK-END BUILTIN
// SLATE-FILECHECK-BEGIN WERROR_INT
// WERROR_INT: Error:   × a type specifier is required for all declarations
// WERROR_INT:   ╰─▶ a type specifier is required for all declarations
// WERROR_INT:    ╭─[tests/fixtures/error/clang/linux/x86_64/implicit_function_modes.c:3:8]
// WERROR_INT:  2 │ #if defined(WERROR_INT) || defined(IMPLICIT_INT)
// WERROR_INT:  3 │ static counter;
// WERROR_INT:    ·        ───────
// WERROR_INT:  4 │ #elif defined(BUILTIN)
// WERROR_INT:    ╰────
// SLATE-FILECHECK-END WERROR_INT
// SLATE-FILECHECK-BEGIN IMPLICIT_INT
// IMPLICIT_INT: Error:   × a type specifier is required for all declarations
// IMPLICIT_INT:   ╰─▶ a type specifier is required for all declarations
// IMPLICIT_INT:    ╭─[tests/fixtures/error/clang/linux/x86_64/implicit_function_modes.c:3:8]
// IMPLICIT_INT:  2 │ #if defined(WERROR_INT) || defined(IMPLICIT_INT)
// IMPLICIT_INT:  3 │ static counter;
// IMPLICIT_INT:    ·        ───────
// IMPLICIT_INT:  4 │ #elif defined(BUILTIN)
// IMPLICIT_INT:    ╰────
// SLATE-FILECHECK-END IMPLICIT_INT
