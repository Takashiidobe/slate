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
// SLATE-FILECHECK-BEGIN IR-RETURN_SIGN
// IR-RETURN_SIGN: module {
// IR-RETURN_SIGN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-RETURN_SIGN-NEXT:         endian = little;
// IR-RETURN_SIGN-NEXT:         pointer [size=8, align=8];
// IR-RETURN_SIGN-NEXT:         stack_alignment = 16;
// IR-RETURN_SIGN-NEXT:         long_double = f80;
// IR-RETURN_SIGN-NEXT:         storage bool [size=1, align=1];
// IR-RETURN_SIGN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-RETURN_SIGN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-RETURN_SIGN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-RETURN_SIGN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-RETURN_SIGN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-RETURN_SIGN-NEXT:         storage bf16 [size=2, align=2];
// IR-RETURN_SIGN-NEXT:         storage f16 [size=2, align=2];
// IR-RETURN_SIGN-NEXT:         storage f32 [size=4, align=4];
// IR-RETURN_SIGN-NEXT:         storage f64 [size=8, align=8];
// IR-RETURN_SIGN-NEXT:         storage f80 [size=16, align=16];
// IR-RETURN_SIGN-NEXT:         storage f128 [size=16, align=16];
// IR-RETURN_SIGN-NEXT:         storage d32 [size=4, align=4];
// IR-RETURN_SIGN-NEXT:         storage d64 [size=8, align=8];
// IR-RETURN_SIGN-NEXT:         storage d128 [size=16, align=16];
// IR-RETURN_SIGN-NEXT:     }
// IR-RETURN_SIGN-NEXT:     fn %0 @f(%1 <unnamed>: i32) -> i32 [linkage=external];
// IR-RETURN_SIGN-NEXT: }
// SLATE-FILECHECK-END IR-RETURN_SIGN
// SLATE-FILECHECK-BEGIN IR-PARAMETERS
// IR-PARAMETERS: module {
// IR-PARAMETERS-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-PARAMETERS-NEXT:         endian = little;
// IR-PARAMETERS-NEXT:         pointer [size=8, align=8];
// IR-PARAMETERS-NEXT:         stack_alignment = 16;
// IR-PARAMETERS-NEXT:         long_double = f80;
// IR-PARAMETERS-NEXT:         storage bool [size=1, align=1];
// IR-PARAMETERS-NEXT:         storage i8, u8 [size=1, align=1];
// IR-PARAMETERS-NEXT:         storage i16, u16 [size=2, align=2];
// IR-PARAMETERS-NEXT:         storage i32, u32 [size=4, align=4];
// IR-PARAMETERS-NEXT:         storage i64, u64 [size=8, align=8];
// IR-PARAMETERS-NEXT:         storage i128, u128 [size=16, align=16];
// IR-PARAMETERS-NEXT:         storage bf16 [size=2, align=2];
// IR-PARAMETERS-NEXT:         storage f16 [size=2, align=2];
// IR-PARAMETERS-NEXT:         storage f32 [size=4, align=4];
// IR-PARAMETERS-NEXT:         storage f64 [size=8, align=8];
// IR-PARAMETERS-NEXT:         storage f80 [size=16, align=16];
// IR-PARAMETERS-NEXT:         storage f128 [size=16, align=16];
// IR-PARAMETERS-NEXT:         storage d32 [size=4, align=4];
// IR-PARAMETERS-NEXT:         storage d64 [size=8, align=8];
// IR-PARAMETERS-NEXT:         storage d128 [size=16, align=16];
// IR-PARAMETERS-NEXT:     }
// IR-PARAMETERS-NEXT:     fn %0 @f(%3 <unnamed>: i32) -> i32 [linkage=external];
// IR-PARAMETERS-NEXT:     fn %1 @g(%5 <unnamed>: i32, %6 <unnamed>: i32) -> i32 [linkage=external];
// IR-PARAMETERS-NEXT:     fn %2 @h(%8 <unnamed>: i32) -> i32 [linkage=external];
// IR-PARAMETERS-NEXT: }
// SLATE-FILECHECK-END IR-PARAMETERS
// SLATE-FILECHECK-BEGIN IR-GLOBAL_SIGN
// IR-GLOBAL_SIGN: module {
// IR-GLOBAL_SIGN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-GLOBAL_SIGN-NEXT:         endian = little;
// IR-GLOBAL_SIGN-NEXT:         pointer [size=8, align=8];
// IR-GLOBAL_SIGN-NEXT:         stack_alignment = 16;
// IR-GLOBAL_SIGN-NEXT:         long_double = f80;
// IR-GLOBAL_SIGN-NEXT:         storage bool [size=1, align=1];
// IR-GLOBAL_SIGN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-GLOBAL_SIGN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-GLOBAL_SIGN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-GLOBAL_SIGN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-GLOBAL_SIGN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-GLOBAL_SIGN-NEXT:         storage bf16 [size=2, align=2];
// IR-GLOBAL_SIGN-NEXT:         storage f16 [size=2, align=2];
// IR-GLOBAL_SIGN-NEXT:         storage f32 [size=4, align=4];
// IR-GLOBAL_SIGN-NEXT:         storage f64 [size=8, align=8];
// IR-GLOBAL_SIGN-NEXT:         storage f80 [size=16, align=16];
// IR-GLOBAL_SIGN-NEXT:         storage f128 [size=16, align=16];
// IR-GLOBAL_SIGN-NEXT:         storage d32 [size=4, align=4];
// IR-GLOBAL_SIGN-NEXT:         storage d64 [size=8, align=8];
// IR-GLOBAL_SIGN-NEXT:         storage d128 [size=16, align=16];
// IR-GLOBAL_SIGN-NEXT:     }
// IR-GLOBAL_SIGN-NEXT:     global %0 x: i32 [storage=static] [linkage=external];
// IR-GLOBAL_SIGN-NEXT: }
// SLATE-FILECHECK-END IR-GLOBAL_SIGN
