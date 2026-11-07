// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR

typedef __Float64x1_t f64x1;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × unknown type name
// ERROR:    ╭─[tests/fixtures/error/clang/linux/aarch64/gcc_only_neon_type.c:2:1]
// ERROR:  1 │
// ERROR:  2 │ typedef __Float64x1_t f64x1;
// ERROR:    · ────────────────────────────
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
