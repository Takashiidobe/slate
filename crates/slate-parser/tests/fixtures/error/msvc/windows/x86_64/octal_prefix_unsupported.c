// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

int x = 0o17;

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid floating literal `0o17`
// SEMANTIC: ╭─[tests/fixtures/error/msvc/windows/x86_64/octal_prefix_unsupported.c:2:9]
// SEMANTIC: 1 │
// SEMANTIC: 2 │ int x = 0o17;
// SEMANTIC: ·         ────
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
