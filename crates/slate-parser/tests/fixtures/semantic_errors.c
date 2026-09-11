void invalid_object;
MissingType missing;

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × object cannot have type void
// SEMANTIC: ╭─[tests/fixtures/semantic_errors.c:1:1]
// SEMANTIC: 1 │ void invalid_object;
// SEMANTIC: · ─
// SEMANTIC: 2 │ MissingType missing;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × unknown type name `MissingType`
// SEMANTIC: ╭─[tests/fixtures/semantic_errors.c:2:1]
// SEMANTIC: 1 │ void invalid_object;
// SEMANTIC: 2 │ MissingType missing;
// SEMANTIC: · ─
// SEMANTIC: 3 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
