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
// BOOLEAN: Error:   × invalid in this context: vector element must be an integer or real
// SLATE-FILECHECK-END BOOLEAN
// SLATE-FILECHECK-BEGIN DECIMAL
// DECIMAL: Error:   × invalid in this context: vector element must be an integer or real
// SLATE-FILECHECK-END DECIMAL
// SLATE-FILECHECK-BEGIN PARTIAL
// PARTIAL: Error:   × invalid in this context: vector_size must be a multiple of the element
// SLATE-FILECHECK-END PARTIAL
// SLATE-FILECHECK-BEGIN EMPTY
// EMPTY: Error:   × invalid in this context: vector_size must be a positive constant
// SLATE-FILECHECK-END EMPTY
// SLATE-FILECHECK-BEGIN LANES
// LANES: Error:   × invalid in this context: ext_vector_type must be a positive constant
// SLATE-FILECHECK-END LANES
// SLATE-FILECHECK-BEGIN RESIZE
// RESIZE: Error:   × invalid in this context: conversion between vector types of different size
// SLATE-FILECHECK-END RESIZE
// SLATE-FILECHECK-BEGIN REMAINDER
// REMAINDER: Error:   × invalid in this context: operator requires integer vector elements
// SLATE-FILECHECK-END REMAINDER
// SLATE-FILECHECK-BEGIN COMPLEMENT
// COMPLEMENT: Error:   × invalid in this context: bitwise complement of a floating vector
// SLATE-FILECHECK-END COMPLEMENT
// SLATE-FILECHECK-BEGIN CONDITION
// CONDITION: Error:   × unsupported in numeric IR lowering: non-scalar condition
// SLATE-FILECHECK-END CONDITION
// SLATE-FILECHECK-BEGIN LOGICAL
// LOGICAL: Error:   × unsupported in numeric IR lowering: non-scalar condition
// SLATE-FILECHECK-END LOGICAL
