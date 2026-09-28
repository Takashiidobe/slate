// SLATE-FILECHECK-DEFINES ENUM ENUM
// SLATE-FILECHECK-DEFINES BITFIELD BITFIELD
// SLATE-FILECHECK-DEFINES ARRAY ARRAY
// SLATE-FILECHECK-DEFINES ASSERT ASSERT
// SLATE-FILECHECK-DEFINES GLOBAL GLOBAL
// SLATE-FILECHECK-DEFINES LOCAL LOCAL
// SLATE-FILECHECK-DEFINES PP PP
// SLATE-FILECHECK-ERROR ENUM
// SLATE-FILECHECK-ERROR BITFIELD
// SLATE-FILECHECK-ERROR ARRAY
// SLATE-FILECHECK-ERROR ASSERT
// SLATE-FILECHECK-ERROR GLOBAL
// SLATE-FILECHECK-ERROR LOCAL
// SLATE-FILECHECK-ERROR PP
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef ENUM
enum E { BAD = 1 / 0 };
#endif

#ifdef BITFIELD
struct B { int x : 1 / 0; };
#endif

#ifdef ARRAY
int bad_array[1 / 0];
#endif

#ifdef ASSERT
_Static_assert(1 / 0, "");
#endif

#ifdef GLOBAL
int bad_global = 1 / 0;
#endif

#ifdef LOCAL
void bad_local(void) { static int x = 1 / 0; }
#endif

#ifdef PP
#if 1 / 0
int bad_pp;
#endif
#endif

// SLATE-FILECHECK-BEGIN ENUM
// ENUM: Error:   × semantic analysis failed
// ENUM: Error:
// ENUM: × unsupported in numeric IR lowering: nonconstant or undefined integer
// ENUM: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:3:1]
// ENUM: 2 │ #ifdef ENUM
// ENUM: 3 │ enum E { BAD = 1 / 0 };
// ENUM: · ───────────────────────
// ENUM: 4 │ #endif
// ENUM: ╰────
// SLATE-FILECHECK-END ENUM
// SLATE-FILECHECK-BEGIN BITFIELD
// BITFIELD: Error:   × semantic analysis failed
// BITFIELD: Error:
// BITFIELD: × unsupported in numeric IR lowering: nonconstant or undefined integer
// BITFIELD: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:7:1]
// BITFIELD: 6 │ #ifdef BITFIELD
// BITFIELD: 7 │ struct B { int x : 1 / 0; };
// BITFIELD: · ────────────────────────────
// BITFIELD: 8 │ #endif
// BITFIELD: ╰────
// SLATE-FILECHECK-END BITFIELD
// SLATE-FILECHECK-BEGIN ARRAY
// ARRAY: Error:   × semantic analysis failed
// ARRAY: Error:
// ARRAY: × unsupported in numeric IR lowering: nonconstant or undefined integer
// ARRAY: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:11:1]
// ARRAY: 10 │ #ifdef ARRAY
// ARRAY: 11 │ int bad_array[1 / 0];
// ARRAY: · ─────────────────────
// ARRAY: 12 │ #endif
// ARRAY: ╰────
// SLATE-FILECHECK-END ARRAY
// SLATE-FILECHECK-BEGIN ASSERT
// ASSERT: Error:   × semantic analysis failed
// ASSERT: Error:
// ASSERT: × static assertion requires an integer constant expression: nonconstant or
// ASSERT: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:15:16]
// ASSERT: 14 │ #ifdef ASSERT
// ASSERT: 15 │ _Static_assert(1 / 0, "");
// ASSERT: ·                ─────
// ASSERT: 16 │ #endif
// ASSERT: ╰────
// SLATE-FILECHECK-END ASSERT
// SLATE-FILECHECK-BEGIN GLOBAL
// GLOBAL: Error:   × semantic analysis failed
// GLOBAL: Error:
// GLOBAL: × initializer element is not a compile-time constant: division by zero
// GLOBAL: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:19:18]
// GLOBAL: 18 │ #ifdef GLOBAL
// GLOBAL: 19 │ int bad_global = 1 / 0;
// GLOBAL: ·                  ─────
// GLOBAL: 20 │ #endif
// GLOBAL: ╰────
// SLATE-FILECHECK-END GLOBAL
// SLATE-FILECHECK-BEGIN LOCAL
// LOCAL: Error:   × semantic analysis failed
// LOCAL: Error:
// LOCAL: × initializer element is not a compile-time constant: division by zero
// LOCAL: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:23:39]
// LOCAL: 22 │ #ifdef LOCAL
// LOCAL: 23 │ void bad_local(void) { static int x = 1 / 0; }
// LOCAL: ·                                       ─────
// LOCAL: 24 │ #endif
// LOCAL: ╰────
// SLATE-FILECHECK-END LOCAL
// SLATE-FILECHECK-BEGIN PP
// PP: Error:   × invalid #if expression: invalid integer constant expression
// PP: ╰─▶ invalid #if expression: invalid integer constant expression
// PP: ╭─[tests/fixtures/sema/constant_arithmetic_rejected.c:27:5]
// PP: 26 │ #ifdef PP
// PP: 27 │ #if 1 / 0
// PP: ·     ─────
// PP: 28 │ int bad_pp;
// PP: ╰────
// SLATE-FILECHECK-END PP
