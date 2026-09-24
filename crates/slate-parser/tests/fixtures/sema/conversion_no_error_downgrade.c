// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -Wno-error=int-conversion

// -Wno-error=<w> demotes a default-error warning without disabling it, in
// both clang and gcc. -W<w> on its own leaves it an error.
int *to_pointer(int value) { return value; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wint-conversion
// WARN: ⚠ incompatible integer to pointer conversion
// WARN: ╭─[tests/fixtures/sema/conversion_no_error_downgrade.c:4:37]
// WARN: 3 │ // both clang and gcc. -W<w> on its own leaves it an error.
// WARN: 4 │ int *to_pointer(int value) { return value; }
// WARN: ·                                     ─────
// WARN: 5 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
