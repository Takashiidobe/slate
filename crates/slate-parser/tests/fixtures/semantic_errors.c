void invalid_object;
MissingType missing;

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error: object cannot have type void
// SEMANTIC: Error: unknown type name `MissingType`
// SEMANTIC: Error:   × semantic analysis failed
// SLATE-FILECHECK-END SEMANTIC
