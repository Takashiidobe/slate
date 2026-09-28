// SLATE-FILECHECK-DEFINES BOOLEAN BOOLEAN
// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES PARTIAL PARTIAL
// SLATE-FILECHECK-DEFINES EMPTY EMPTY
// SLATE-FILECHECK-DEFINES LANES LANES
// SLATE-FILECHECK-DEFINES RESIZE RESIZE
// SLATE-FILECHECK-DEFINES REMAINDER REMAINDER
// SLATE-FILECHECK-DEFINES COMPLEMENT COMPLEMENT
// SLATE-FILECHECK-DEFINES CONDITION CONDITION
// SLATE-FILECHECK-DEFINES LOGICAL LOGICAL
// SLATE-FILECHECK-ERROR BOOLEAN
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR PARTIAL
// SLATE-FILECHECK-ERROR EMPTY
// SLATE-FILECHECK-ERROR LANES
// SLATE-FILECHECK-ERROR RESIZE
// SLATE-FILECHECK-ERROR REMAINDER
// SLATE-FILECHECK-ERROR COMPLEMENT
// SLATE-FILECHECK-ERROR CONDITION
// SLATE-FILECHECK-ERROR LOGICAL
// SLATE-FILECHECK-ARGS --dump-ir

typedef int v4si __attribute__((vector_size(16)));
typedef int v2si __attribute__((vector_size(8)));
typedef float v4sf __attribute__((vector_size(16)));

#ifdef BOOLEAN
typedef _Bool v4b __attribute__((vector_size(4)));
v4b boolean;
#endif
#ifdef DECIMAL
typedef _Decimal64 v2d __attribute__((vector_size(16)));
v2d decimal;
#endif
#ifdef PARTIAL
typedef int v3si __attribute__((vector_size(6)));
v3si partial;
#endif
#ifdef EMPTY
typedef int v0si __attribute__((vector_size(0)));
v0si empty;
#endif
#ifdef LANES
typedef int v0ext __attribute__((ext_vector_type(0)));
v0ext lanes;
#endif

v4si invalid(v4si a, v2si b, v4sf c) {
#ifdef RESIZE
    return b;
#endif
#ifdef REMAINDER
    return c % c;
#endif
#ifdef COMPLEMENT
    return ~c;
#endif
#ifdef CONDITION
    if (a) {
        return a;
    }
#endif
#ifdef LOGICAL
    return a && a;
#endif
    return a;
}

// SLATE-FILECHECK-BEGIN BOOLEAN
// BOOLEAN: Error:   × semantic analysis failed
// BOOLEAN: Error:
// BOOLEAN: × invalid in this context: vector element must be an integer or real
// BOOLEAN: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:7:1]
// BOOLEAN: 6 │ #ifdef BOOLEAN
// BOOLEAN: 7 │ typedef _Bool v4b __attribute__((vector_size(4)));
// BOOLEAN: · ──────────────────────────────────────────────────
// BOOLEAN: 8 │ v4b boolean;
// BOOLEAN: ╰────
// SLATE-FILECHECK-END BOOLEAN
// SLATE-FILECHECK-BEGIN DECIMAL
// DECIMAL: Error:   × semantic analysis failed
// DECIMAL: Error:
// DECIMAL: × invalid in this context: vector element must be an integer or real
// DECIMAL: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:11:1]
// DECIMAL: 10 │ #ifdef DECIMAL
// DECIMAL: 11 │ typedef _Decimal64 v2d __attribute__((vector_size(16)));
// DECIMAL: · ────────────────────────────────────────────────────────
// DECIMAL: 12 │ v2d decimal;
// DECIMAL: ╰────
// SLATE-FILECHECK-END DECIMAL
// SLATE-FILECHECK-BEGIN PARTIAL
// PARTIAL: Error:   × semantic analysis failed
// PARTIAL: Error:
// PARTIAL: × invalid in this context: vector_size must be a multiple of the element
// PARTIAL: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:15:1]
// PARTIAL: 14 │ #ifdef PARTIAL
// PARTIAL: 15 │ typedef int v3si __attribute__((vector_size(6)));
// PARTIAL: · ─────────────────────────────────────────────────
// PARTIAL: 16 │ v3si partial;
// PARTIAL: ╰────
// SLATE-FILECHECK-END PARTIAL
// SLATE-FILECHECK-BEGIN EMPTY
// EMPTY: Error:   × semantic analysis failed
// EMPTY: Error:
// EMPTY: × invalid in this context: vector_size must be a positive constant
// EMPTY: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:19:1]
// EMPTY: 18 │ #ifdef EMPTY
// EMPTY: 19 │ typedef int v0si __attribute__((vector_size(0)));
// EMPTY: · ─────────────────────────────────────────────────
// EMPTY: 20 │ v0si empty;
// EMPTY: ╰────
// SLATE-FILECHECK-END EMPTY
// SLATE-FILECHECK-BEGIN LANES
// LANES: Error:   × semantic analysis failed
// LANES: Error:
// LANES: × invalid in this context: ext_vector_type must be a positive constant
// LANES: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:23:1]
// LANES: 22 │ #ifdef LANES
// LANES: 23 │ typedef int v0ext __attribute__((ext_vector_type(0)));
// LANES: · ──────────────────────────────────────────────────────
// LANES: 24 │ v0ext lanes;
// LANES: ╰────
// SLATE-FILECHECK-END LANES
// SLATE-FILECHECK-BEGIN RESIZE
// RESIZE: Error:   × semantic analysis failed
// RESIZE: Error:
// RESIZE: × invalid in this context: conversion between vector types of different size
// RESIZE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:29:5]
// RESIZE: 28 │ #ifdef RESIZE
// RESIZE: 29 │     return b;
// RESIZE: ·     ─────────
// RESIZE: 30 │ #endif
// RESIZE: ╰────
// SLATE-FILECHECK-END RESIZE
// SLATE-FILECHECK-BEGIN REMAINDER
// REMAINDER: Error:   × semantic analysis failed
// REMAINDER: Error:
// REMAINDER: × invalid in this context: operator requires integer vector elements
// REMAINDER: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:32:12]
// REMAINDER: 31 │ #ifdef REMAINDER
// REMAINDER: 32 │     return c % c;
// REMAINDER: ·            ─────
// REMAINDER: 33 │ #endif
// REMAINDER: ╰────
// SLATE-FILECHECK-END REMAINDER
// SLATE-FILECHECK-BEGIN COMPLEMENT
// COMPLEMENT: Error:   × semantic analysis failed
// COMPLEMENT: Error:
// COMPLEMENT: × invalid in this context: bitwise complement of a floating vector
// COMPLEMENT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:35:12]
// COMPLEMENT: 34 │ #ifdef COMPLEMENT
// COMPLEMENT: 35 │     return ~c;
// COMPLEMENT: ·            ──
// COMPLEMENT: 36 │ #endif
// COMPLEMENT: ╰────
// SLATE-FILECHECK-END COMPLEMENT
// SLATE-FILECHECK-BEGIN CONDITION
// CONDITION: Error:   × semantic analysis failed
// CONDITION: Error:
// CONDITION: × unsupported in numeric IR lowering: non-scalar condition
// CONDITION: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:38:5]
// CONDITION: 37 │     #ifdef CONDITION
// CONDITION: 38 │ ╭─▶     if (a) {
// CONDITION: 39 │ │           return a;
// CONDITION: 40 │ ╰─▶     }
// CONDITION: 41 │     #endif
// CONDITION: ╰────
// SLATE-FILECHECK-END CONDITION
// SLATE-FILECHECK-BEGIN LOGICAL
// LOGICAL: Error:   × semantic analysis failed
// LOGICAL: Error:
// LOGICAL: × unsupported in numeric IR lowering: non-scalar condition
// LOGICAL: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_vector_invalid.c:43:12]
// LOGICAL: 42 │ #ifdef LOGICAL
// LOGICAL: 43 │     return a && a;
// LOGICAL: ·            ──────
// LOGICAL: 44 │ #endif
// LOGICAL: ╰────
// SLATE-FILECHECK-END LOGICAL
