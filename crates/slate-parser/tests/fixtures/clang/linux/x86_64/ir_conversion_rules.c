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
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_conversion_rules.c:5:53]
// WARN: 4 │ // flavor-aware; see wiki/concepts/diagnostic-severity.md.
// WARN: 5 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: ·                                                     ─────
// WARN: 6 │ signed char *plain_char(char *value) { return value; }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_conversion_rules.c:6:47]
// WARN: 5 │ int **nested_qualifiers(const int **value) { return value; }
// WARN: 6 │ signed char *plain_char(char *value) { return value; }
// WARN: ·                                               ─────
// WARN: 7 │ char *signed_char(signed char *value) { return value; }
// WARN: ╰────
// WARN: -Wpointer-sign
// WARN: ⚠ conversion between pointers to integer types with different sign
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_conversion_rules.c:7:48]
// WARN: 6 │ signed char *plain_char(char *value) { return value; }
// WARN: 7 │ char *signed_char(signed char *value) { return value; }
// WARN: ·                                                ─────
// WARN: 8 │ int *discards_const(const int *value) { return value; }
// WARN: ╰────
// WARN: -Wincompatible-pointer-types-discards-qualifiers
// WARN: ⚠ pointer conversion discards qualifiers
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/ir_conversion_rules.c:8:48]
// WARN: 7 │ char *signed_char(signed char *value) { return value; }
// WARN: 8 │ int *discards_const(const int *value) { return value; }
// WARN: ·                                                ─────
// WARN: 9 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f80;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f80 [size=16, align=16];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %0 @nested_qualifiers(%1 value: ptr<ptr<const i32>>) -> ptr<ptr<i32>> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return pointer_cast<ptr<ptr<i32>>, reason=return>(read<ptr<ptr<const i32>>>(%1));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %2 @plain_char(%3 value: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return pointer_cast<ptr<i8>, reason=return>(read<ptr<i8>>(%3));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %4 @signed_char(%5 value: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return pointer_cast<ptr<i8>, reason=return>(read<ptr<i8>>(%5));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %6 @discards_const(%7 value: ptr<const i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return pointer_cast<ptr<i32>, reason=return>(read<ptr<const i32>>(%7));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
