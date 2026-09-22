// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// Every attribute here is written on a subject outside clang's subject list
// for it, so clang drops it under -Wignored-attributes rather than applying
// it or rejecting the declaration. Measured against clang 22.1.8.

__attribute__((transparent_union)) int not_a_union;
__attribute__((ms_struct)) int not_a_record;
__attribute__((ifunc("resolver"))) int not_a_function;
__attribute__((malloc)) int not_allocating;
__attribute__((nocommon)) void not_a_variable(void) {}
void cleanup_target(void *p);
__attribute__((cleanup(cleanup_target))) int not_a_local;
typedef __attribute__((visibility("hidden"))) int hidden_alias;

int uses(hidden_alias value) { return value + not_a_union + not_a_local; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ 'transparent_union' attribute ignored; it applies only to unions
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:6:16]
// WARN: 5 │
// WARN: 6 │ __attribute__((transparent_union)) int not_a_union;
// WARN: ·                ─────────────────
// WARN: 7 │ __attribute__((ms_struct)) int not_a_record;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'ms_struct' attribute ignored; it applies only to structs, unions, and
// WARN: │ classes
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:7:16]
// WARN: 6 │ __attribute__((transparent_union)) int not_a_union;
// WARN: 7 │ __attribute__((ms_struct)) int not_a_record;
// WARN: ·                ─────────
// WARN: 8 │ __attribute__((ifunc("resolver"))) int not_a_function;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'ifunc' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:8:16]
// WARN: 7 │ __attribute__((ms_struct)) int not_a_record;
// WARN: 8 │ __attribute__((ifunc("resolver"))) int not_a_function;
// WARN: ·                ─────
// WARN: 9 │ __attribute__((malloc)) int not_allocating;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'malloc' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:9:16]
// WARN: 8 │ __attribute__((ifunc("resolver"))) int not_a_function;
// WARN: 9 │ __attribute__((malloc)) int not_allocating;
// WARN: ·                ──────
// WARN: 10 │ __attribute__((nocommon)) void not_a_variable(void) {}
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'nocommon' attribute ignored; it applies only to variables
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:10:16]
// WARN: 9 │ __attribute__((malloc)) int not_allocating;
// WARN: 10 │ __attribute__((nocommon)) void not_a_variable(void) {}
// WARN: ·                ────────
// WARN: 11 │ void cleanup_target(void *p);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'cleanup' attribute ignored; it applies only to local variables
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:12:16]
// WARN: 11 │ void cleanup_target(void *p);
// WARN: 12 │ __attribute__((cleanup(cleanup_target))) int not_a_local;
// WARN: ·                ───────
// WARN: 13 │ typedef __attribute__((visibility("hidden"))) int hidden_alias;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'visibility' attribute ignored
// WARN: ╭─[tests/fixtures/sema/attribute_applicability_warnings.c:13:24]
// WARN: 12 │ __attribute__((cleanup(cleanup_target))) int not_a_local;
// WARN: 13 │ typedef __attribute__((visibility("hidden"))) int hidden_alias;
// WARN: ·                        ──────────
// WARN: 14 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
