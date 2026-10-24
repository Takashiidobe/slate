// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

unsigned long long f(void *p) { return sizeof(*p); }

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC:   × indirection of a void pointer
// SEMANTIC:    ╭─[tests/fixtures/error/msvc/windows/x86_64/void_dereference.c:2:47]
// SEMANTIC:  1 │
// SEMANTIC:  2 │ unsigned long long f(void *p) { return sizeof(*p); }
// SEMANTIC:    ·                                               ──
// SEMANTIC:  3 │
// SEMANTIC:    ╰────
// SLATE-FILECHECK-END SEMANTIC
