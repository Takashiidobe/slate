// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

void take(int *);
unsigned *returned(int *value) { return value; }
void passed(unsigned *value) { take(value); }
void assigned(unsigned *value, const int *constant, volatile int *shared, const int **nested) {
    int *sign = value;
    int *dropped_const = constant;
    int *dropped_volatile = shared;
    int **dropped_nested = nested;
    const unsigned *added_const = sign;
    unsigned *explicit_cast = (unsigned *)sign;
    const int *compatible = sign;
    (void)dropped_const, (void)dropped_volatile, (void)dropped_nested;
    (void)added_const, (void)explicit_cast, (void)compatible;
}
int compared(int *left, unsigned *right) { return left == (int *)right; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:3:41]
// WARN: 2 │ void take(int *);
// WARN: 3 │ unsigned *returned(int *value) { return value; }
// WARN: ·                                         ─────
// WARN: 4 │ void passed(unsigned *value) { take(value); }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:4:37]
// WARN: 3 │ unsigned *returned(int *value) { return value; }
// WARN: 4 │ void passed(unsigned *value) { take(value); }
// WARN: ·                                     ─────
// WARN: 5 │ void assigned(unsigned *value, const int *constant, volatile int *shared, const int **nested) {
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:6:17]
// WARN: 5 │ void assigned(unsigned *value, const int *constant, volatile int *shared, const int **nested) {
// WARN: 6 │     int *sign = value;
// WARN: ·                 ─────
// WARN: 7 │     int *dropped_const = constant;
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:7:26]
// WARN: 6 │     int *sign = value;
// WARN: 7 │     int *dropped_const = constant;
// WARN: ·                          ────────
// WARN: 8 │     int *dropped_volatile = shared;
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:8:29]
// WARN: 7 │     int *dropped_const = constant;
// WARN: 8 │     int *dropped_volatile = shared;
// WARN: ·                             ──────
// WARN: 9 │     int **dropped_nested = nested;
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers in nested pointer types
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:9:28]
// WARN: 8 │     int *dropped_volatile = shared;
// WARN: 9 │     int **dropped_nested = nested;
// WARN: ·                            ──────
// WARN: 10 │     const unsigned *added_const = sign;
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/sema/ir_pointer_conversion_warnings.c:10:35]
// WARN: 9 │     int **dropped_nested = nested;
// WARN: 10 │     const unsigned *added_const = sign;
// WARN: ·                                   ────
// WARN: 11 │     unsigned *explicit_cast = (unsigned *)sign;
// WARN: ╰────
// SLATE-FILECHECK-END WARN
