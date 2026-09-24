// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

// Conversions that stay warnings under every flavor. The ones clang and gcc
// reject moved to error/conversion_default_errors.c when severity became
// flavor-aware; see wiki/concepts/diagnostic-severity.md.
int **nested_qualifiers(const int **value) { return value; }
signed char *plain_char(char *value) { return value; }
char *signed_char(signed char *value) { return value; }
int *discards_const(const int *value) { return value; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers in nested pointer types
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:5:53]
// WARN: 4 │ // flavor-aware; see wiki/concepts/diagnostic-severity.md.
// WARN: 5 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: ·                                                     ─────
// WARN: 6 │ signed char *plain_char(char *value) { return value; }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:6:47]
// WARN: 5 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: 6 │ signed char *plain_char(char *value) { return value; }
// WARN: ·                                               ─────
// WARN: 7 │ char *signed_char(signed char *value) { return value; }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:7:48]
// WARN: 6 │ signed char *plain_char(char *value) { return value; }
// WARN: 7 │ char *signed_char(signed char *value) { return value; }
// WARN: ·                                                ─────
// WARN: 8 │ int *discards_const(const int *value) { return value; }
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:8:48]
// WARN: 7 │ char *signed_char(signed char *value) { return value; }
// WARN: 8 │ int *discards_const(const int *value) { return value; }
// WARN: ·                                                ─────
// WARN: 9 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
