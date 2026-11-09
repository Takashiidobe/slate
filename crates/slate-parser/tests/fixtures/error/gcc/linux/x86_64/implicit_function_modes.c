// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 gnu17
// SLATE-FILECHECK-ERROR C99
// SLATE-FILECHECK-DEFINES NO_ERROR_GROUP
// SLATE-FILECHECK-STD NO_ERROR_GROUP gnu23
// SLATE-FILECHECK-PREFIX-ARGS NO_ERROR_GROUP -Wno-error=implicit
// SLATE-FILECHECK-ERROR NO_ERROR_GROUP
// SLATE-FILECHECK-DEFINES WERROR_INT WERROR_INT
// SLATE-FILECHECK-STD WERROR_INT c89
// SLATE-FILECHECK-PREFIX-ARGS WERROR_INT -Werror=implicit-int
// SLATE-FILECHECK-ERROR WERROR_INT
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef WERROR_INT
static counter;
#else
int f(void) { return undeclared(1); }
#endif

// SLATE-FILECHECK-BEGIN C99
// C99: Error:   × semantic analysis failed
// C99: Error: -Wimplicit-function-declaration
// C99:   × implicit declaration of function 'undeclared'
// C99:    ╭─[tests/fixtures/error/gcc/linux/x86_64/implicit_function_modes.c:5:22]
// C99:  4 │ #else
// C99:  5 │ int f(void) { return undeclared(1); }
// C99:    ·                      ──────────
// C99:  6 │ #endif
// C99:    ╰────
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN NO_ERROR_GROUP
// NO_ERROR_GROUP: Error:   × semantic analysis failed
// NO_ERROR_GROUP: Error: -Wimplicit-function-declaration
// NO_ERROR_GROUP:   × implicit declaration of function 'undeclared'
// NO_ERROR_GROUP:    ╭─[tests/fixtures/error/gcc/linux/x86_64/implicit_function_modes.c:5:22]
// NO_ERROR_GROUP:  4 │ #else
// NO_ERROR_GROUP:  5 │ int f(void) { return undeclared(1); }
// NO_ERROR_GROUP:    ·                      ──────────
// NO_ERROR_GROUP:  6 │ #endif
// NO_ERROR_GROUP:    ╰────
// SLATE-FILECHECK-END NO_ERROR_GROUP
// SLATE-FILECHECK-BEGIN WERROR_INT
// WERROR_INT: Error:   × a type specifier is required for all declarations
// WERROR_INT:   ╰─▶ a type specifier is required for all declarations
// WERROR_INT:    ╭─[tests/fixtures/error/gcc/linux/x86_64/implicit_function_modes.c:3:8]
// WERROR_INT:  2 │ #ifdef WERROR_INT
// WERROR_INT:  3 │ static counter;
// WERROR_INT:    ·        ───────
// WERROR_INT:  4 │ #else
// WERROR_INT:    ╰────
// SLATE-FILECHECK-END WERROR_INT
