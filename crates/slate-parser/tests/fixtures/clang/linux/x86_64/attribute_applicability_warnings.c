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
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:6:16]
// WARN: 5 │
// WARN: 6 │ __attribute__((transparent_union)) int not_a_union;
// WARN: ·                ─────────────────
// WARN: 7 │ __attribute__((ms_struct)) int not_a_record;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'ms_struct' attribute ignored; it applies only to structs, unions, and
// WARN: │ classes
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:7:16]
// WARN: 6 │ __attribute__((transparent_union)) int not_a_union;
// WARN: 7 │ __attribute__((ms_struct)) int not_a_record;
// WARN: ·                ─────────
// WARN: 8 │ __attribute__((ifunc("resolver"))) int not_a_function;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'ifunc' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:8:16]
// WARN: 7 │ __attribute__((ms_struct)) int not_a_record;
// WARN: 8 │ __attribute__((ifunc("resolver"))) int not_a_function;
// WARN: ·                ─────
// WARN: 9 │ __attribute__((malloc)) int not_allocating;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'malloc' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:9:16]
// WARN: 8 │ __attribute__((ifunc("resolver"))) int not_a_function;
// WARN: 9 │ __attribute__((malloc)) int not_allocating;
// WARN: ·                ──────
// WARN: 10 │ __attribute__((nocommon)) void not_a_variable(void) {}
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'nocommon' attribute ignored; it applies only to variables
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:10:16]
// WARN: 9 │ __attribute__((malloc)) int not_allocating;
// WARN: 10 │ __attribute__((nocommon)) void not_a_variable(void) {}
// WARN: ·                ────────
// WARN: 11 │ void cleanup_target(void *p);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'cleanup' attribute ignored; it applies only to local variables
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:12:16]
// WARN: 11 │ void cleanup_target(void *p);
// WARN: 12 │ __attribute__((cleanup(cleanup_target))) int not_a_local;
// WARN: ·                ───────
// WARN: 13 │ typedef __attribute__((visibility("hidden"))) int hidden_alias;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'visibility' attribute ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/attribute_applicability_warnings.c:13:24]
// WARN: 12 │ __attribute__((cleanup(cleanup_target))) int not_a_local;
// WARN: 13 │ typedef __attribute__((visibility("hidden"))) int hidden_alias;
// WARN: ·                        ──────────
// WARN: 14 │
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
// IR-WARN-NEXT:     type @type[[TYPE_hidden_alias:[0-9]+]] hidden_alias = i32;
// IR-WARN-NEXT:     global %[[VALUE_not_a_union:[0-9]+]] not_a_union: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_not_a_record:[0-9]+]] not_a_record: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_not_a_function:[0-9]+]] not_a_function: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_not_allocating:[0-9]+]] not_allocating: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_not_a_local:[0-9]+]] not_a_local: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_not_a_variable:[0-9]+]] @not_a_variable() -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_cleanup_target:[0-9]+]] @cleanup_target(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_uses:[0-9]+]] @uses(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return add<i32>(add<i32>(read<i32>(%[[VALUE_value]]), read<i32>(%[[VALUE_not_a_union]])), read<i32>(%[[VALUE_not_a_local]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
