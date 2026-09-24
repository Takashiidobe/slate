// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir

// MSVC 19.51 reports C4047/C4133 and keeps compiling at every standard, so
// the msvc flavor keeps both as warnings. Measured 2026-09-21.
int *to_pointer(int value) { return value; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wint-conversion
// WARN: ⚠ incompatible integer to pointer conversion
// WARN: ╭─[tests/fixtures/sema/conversion_severity_msvc.c:4:37]
// WARN: 3 │ // the msvc flavor keeps both as warnings. Measured 2026-09-21.
// WARN: 4 │ int *to_pointer(int value) { return value; }
// WARN: ·                                     ─────
// WARN: 5 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
