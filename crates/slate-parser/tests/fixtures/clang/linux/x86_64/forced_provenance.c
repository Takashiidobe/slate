void from_imacros(void) { IMACROS_VALUE; }

// SLATE-FILECHECK-ARGS --dump-ir-expressions --show-spans -imacrostests/fixtures/inputs/forced_inputs/only_macros.h
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: const<i32>(14) [spelling=[[#FILE0:]]:22+2, expansion=[[#FILE1:]]:26+13]
// SLATE-FILECHECK-END DEFAULT
