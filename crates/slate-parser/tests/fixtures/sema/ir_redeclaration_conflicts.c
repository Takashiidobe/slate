// SLATE-FILECHECK-DEFINES RETURN_KIND RETURN_KIND
// SLATE-FILECHECK-ERROR RETURN_KIND
// SLATE-FILECHECK-DEFINES RETURN_SIZE RETURN_SIZE
// SLATE-FILECHECK-ERROR RETURN_SIZE
// SLATE-FILECHECK-DEFINES RETURN_INDIRECTION RETURN_INDIRECTION
// SLATE-FILECHECK-ERROR RETURN_INDIRECTION
// SLATE-FILECHECK-DEFINES RETURN_SIGN RETURN_SIGN
// SLATE-FILECHECK-WARNING RETURN_SIGN
// SLATE-FILECHECK-DEFINES PARAMETERS PARAMETERS
// SLATE-FILECHECK-WARNING PARAMETERS
// SLATE-FILECHECK-DEFINES GLOBAL_SIGN GLOBAL_SIGN
// SLATE-FILECHECK-WARNING GLOBAL_SIGN
// SLATE-FILECHECK-DEFINES STRUCT STRUCT
// SLATE-FILECHECK-ERROR STRUCT
// SLATE-FILECHECK-DEFINES STRUCT_SAME_C17 STRUCT_SAME_C17
// SLATE-FILECHECK-STD STRUCT_SAME_C17 c17
// SLATE-FILECHECK-ERROR STRUCT_SAME_C17
// SLATE-FILECHECK-DEFINES STRUCT_DIFFERENT_C23 STRUCT_DIFFERENT_C23
// SLATE-FILECHECK-STD STRUCT_DIFFERENT_C23 c23
// SLATE-FILECHECK-ERROR STRUCT_DIFFERENT_C23
// SLATE-FILECHECK-DEFINES UNION UNION
// SLATE-FILECHECK-ERROR UNION
// SLATE-FILECHECK-DEFINES ENUM ENUM
// SLATE-FILECHECK-ERROR ENUM
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#if defined(RETURN_KIND)
int f(int);
double f(int);
#elif defined(RETURN_SIZE)
int f(int);
short f(int);
#elif defined(RETURN_INDIRECTION)
int f(void);
char *f(void);
#elif defined(RETURN_SIGN)
int f(int);
unsigned f(int);
#elif defined(PARAMETERS)
int f(int);
int f(double);
int g(int, int);
int g(int);
int h(int);
int h(int, ...);
#elif defined(GLOBAL_SIGN)
int x;
unsigned x;
#elif defined(STRUCT)
struct S { int a; };
struct S { int b; };
#elif defined(STRUCT_SAME_C17)
struct S { int a; };
struct S { int a; };
#elif defined(STRUCT_DIFFERENT_C23)
struct S { int a; };
struct S { long a; };
#elif defined(UNION)
union U { int a; };
union U { int b; };
#elif defined(ENUM)
enum E { A };
enum E { B };
#endif

// SLATE-FILECHECK-BEGIN RETURN_KIND
// RETURN_KIND: Error:   × invalid in this context: conflicting types for function redeclaration
// SLATE-FILECHECK-END RETURN_KIND
// SLATE-FILECHECK-BEGIN RETURN_SIZE
// RETURN_SIZE: Error:   × invalid in this context: conflicting types for function redeclaration
// SLATE-FILECHECK-END RETURN_SIZE
// SLATE-FILECHECK-BEGIN RETURN_INDIRECTION
// RETURN_INDIRECTION: Error:   × invalid in this context: conflicting types for function redeclaration
// SLATE-FILECHECK-END RETURN_INDIRECTION
// SLATE-FILECHECK-BEGIN STRUCT
// STRUCT: Error:   × invalid in this context: redefinition of struct, union, or enum tag
// SLATE-FILECHECK-END STRUCT
// SLATE-FILECHECK-BEGIN STRUCT_SAME_C17
// STRUCT_SAME_C17: Error:   × invalid in this context: redefinition of struct, union, or enum tag
// SLATE-FILECHECK-END STRUCT_SAME_C17
// SLATE-FILECHECK-BEGIN STRUCT_DIFFERENT_C23
// STRUCT_DIFFERENT_C23: Error:   × invalid in this context: redefinition of struct, union, or enum tag
// SLATE-FILECHECK-END STRUCT_DIFFERENT_C23
// SLATE-FILECHECK-BEGIN UNION
// UNION: Error:   × invalid in this context: redefinition of struct, union, or enum tag
// SLATE-FILECHECK-END UNION
// SLATE-FILECHECK-BEGIN ENUM
// ENUM: Error:   × invalid in this context: redefinition of struct, union, or enum tag
// SLATE-FILECHECK-END ENUM
// SLATE-FILECHECK-BEGIN RETURN_SIGN
// RETURN_SIGN: -Wconflicting-types
// RETURN_SIGN: ⚠ function redeclared with a different integer return type of the same size
// RETURN_SIGN: ╭─[tests/fixtures/sema/ir_redeclaration_conflicts.c:13:10]
// RETURN_SIGN: 12 │ int f(int);
// RETURN_SIGN: 13 │ unsigned f(int);
// RETURN_SIGN: ·          ──────
// RETURN_SIGN: 14 │ #elif defined(PARAMETERS)
// RETURN_SIGN: ╰────
// SLATE-FILECHECK-END RETURN_SIGN
// SLATE-FILECHECK-BEGIN PARAMETERS
// PARAMETERS: -Wconflicting-types
// PARAMETERS: ⚠ function redeclared with a different parameter list
// PARAMETERS: ╭─[tests/fixtures/sema/ir_redeclaration_conflicts.c:16:5]
// PARAMETERS: 15 │ int f(int);
// PARAMETERS: 16 │ int f(double);
// PARAMETERS: ·     ─────────
// PARAMETERS: 17 │ int g(int, int);
// PARAMETERS: ╰────
// PARAMETERS: -Wconflicting-types
// PARAMETERS: ⚠ function redeclared with a different parameter list
// PARAMETERS: ╭─[tests/fixtures/sema/ir_redeclaration_conflicts.c:18:5]
// PARAMETERS: 17 │ int g(int, int);
// PARAMETERS: 18 │ int g(int);
// PARAMETERS: ·     ──────
// PARAMETERS: 19 │ int h(int);
// PARAMETERS: ╰────
// PARAMETERS: -Wconflicting-types
// PARAMETERS: ⚠ function redeclared with a different parameter list
// PARAMETERS: ╭─[tests/fixtures/sema/ir_redeclaration_conflicts.c:20:5]
// PARAMETERS: 19 │ int h(int);
// PARAMETERS: 20 │ int h(int, ...);
// PARAMETERS: ·     ───────────
// PARAMETERS: 21 │ #elif defined(GLOBAL_SIGN)
// PARAMETERS: ╰────
// SLATE-FILECHECK-END PARAMETERS
// SLATE-FILECHECK-BEGIN GLOBAL_SIGN
// GLOBAL_SIGN: -Wconflicting-types
// GLOBAL_SIGN: ⚠ redeclaration with a different integer type of the same size
// GLOBAL_SIGN: ╭─[tests/fixtures/sema/ir_redeclaration_conflicts.c:23:10]
// GLOBAL_SIGN: 22 │ int x;
// GLOBAL_SIGN: 23 │ unsigned x;
// GLOBAL_SIGN: ·          ─
// GLOBAL_SIGN: 24 │ #elif defined(STRUCT)
// GLOBAL_SIGN: ╰────
// SLATE-FILECHECK-END GLOBAL_SIGN
