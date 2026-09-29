void invalid_object;
void *valid_pointer;
void *valid_pointer_array[2];
void invalid_array[2];
MissingType missing;
#define BAD_TYPE MissingMacroType
BAD_TYPE macro_missing;

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × object cannot have type void
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/semantic_errors.c:1:6]
// SEMANTIC: 1 │ void invalid_object;
// SEMANTIC: ·      ──────────────
// SEMANTIC: 2 │ void *valid_pointer;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × object cannot have type void
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/semantic_errors.c:4:1]
// SEMANTIC: 3 │ void *valid_pointer_array[2];
// SEMANTIC: 4 │ void invalid_array[2];
// SEMANTIC: · ──────────────────────
// SEMANTIC: 5 │ MissingType missing;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × unknown type name `MissingType`
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/semantic_errors.c:5:1]
// SEMANTIC: 4 │ void invalid_array[2];
// SEMANTIC: 5 │ MissingType missing;
// SEMANTIC: · ───────────
// SEMANTIC: 6 │ #define BAD_TYPE MissingMacroType
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × unknown type name `MissingMacroType`
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/semantic_errors.c:7:1]
// SEMANTIC: 6 │ #define BAD_TYPE MissingMacroType
// SEMANTIC: 7 │ BAD_TYPE macro_missing;
// SEMANTIC: · ────────
// SEMANTIC: 8 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
