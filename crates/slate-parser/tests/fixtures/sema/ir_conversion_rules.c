// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23


long *widened(unsigned *value) { return value; }
int **nested_sign(unsigned **value) { return value; }
struct A {
    int a;
};
struct B {
    int b;
};
struct B *unrelated(struct A *value) { return value; }
int from_pointer(int *value) { return value; }
int *to_pointer(int value) { return value; }
int **nested_qualifiers(const int **value) { return value; }
signed char *plain_char(char *value) { return value; }
char *signed_char(signed char *value) { return value; }
int *discards_const(const int *value) { return value; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wincompatible-pointer-types
// WARN: ⚠ incompatible pointer types
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:3:41]
// WARN: 2 │
// WARN: 3 │ long *widened(unsigned *value) { return value; }
// WARN: ·                                         ─────
// WARN: 4 │ int **nested_sign(unsigned **value) { return value; }
// WARN: ╰────
// WARN: -Wincompatible-pointer-types
// WARN: ⚠ incompatible pointer types
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:4:46]
// WARN: 3 │ long *widened(unsigned *value) { return value; }
// WARN: 4 │ int **nested_sign(unsigned **value) { return value; }
// WARN: ·                                              ─────
// WARN: 5 │ struct A {
// WARN: ╰────
// WARN: -Wincompatible-pointer-types
// WARN: ⚠ incompatible pointer types
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:11:47]
// WARN: 10 │ };
// WARN: 11 │ struct B *unrelated(struct A *value) { return value; }
// WARN: ·                                               ─────
// WARN: 12 │ int from_pointer(int *value) { return value; }
// WARN: ╰────
// WARN: -Wint-conversion
// WARN: ⚠ incompatible pointer to integer conversion
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:12:39]
// WARN: 11 │ struct B *unrelated(struct A *value) { return value; }
// WARN: 12 │ int from_pointer(int *value) { return value; }
// WARN: ·                                       ─────
// WARN: 13 │ int *to_pointer(int value) { return value; }
// WARN: ╰────
// WARN: -Wint-conversion
// WARN: ⚠ incompatible integer to pointer conversion
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:13:37]
// WARN: 12 │ int from_pointer(int *value) { return value; }
// WARN: 13 │ int *to_pointer(int value) { return value; }
// WARN: ·                                     ─────
// WARN: 14 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers in nested pointer types
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:14:53]
// WARN: 13 │ int *to_pointer(int value) { return value; }
// WARN: 14 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: ·                                                     ─────
// WARN: 15 │ signed char *plain_char(char *value) { return value; }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:15:47]
// WARN: 14 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: 15 │ signed char *plain_char(char *value) { return value; }
// WARN: ·                                               ─────
// WARN: 16 │ char *signed_char(signed char *value) { return value; }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:16:48]
// WARN: 15 │ signed char *plain_char(char *value) { return value; }
// WARN: 16 │ char *signed_char(signed char *value) { return value; }
// WARN: ·                                                ─────
// WARN: 17 │ int *discards_const(const int *value) { return value; }
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/sema/ir_conversion_rules.c:17:48]
// WARN: 16 │ char *signed_char(signed char *value) { return value; }
// WARN: 17 │ int *discards_const(const int *value) { return value; }
// WARN: ·                                                ─────
// WARN: 18 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
