// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=c23

__attribute__((dllimport)) int imported;
__attribute__((dllexport)) int exported = 1;
__attribute__((noipa)) void opaque(void) {}
__attribute__((overloadable)) void overloaded(int);
__attribute__((naked)) void bare(void);
__attribute__((not_an_attribute)) int bogus;
[[gnu::not_an_attribute]] int scoped_bogus;
[[clang::overloadable]] void clang_scoped(int);
[[msvc::noinline]] void msvc_scoped(void);

char unterminated[4] __attribute__((nonstring));

#if __has_attribute(dllimport)
int has_dllimport;
#endif
#if __has_attribute(naked)
int has_naked;
#endif
#if __has_attribute(noipa)
int has_noipa;
#endif
#if __has_attribute(nonstring)
int has_nonstring;
#endif
#if __has_attribute(const)
int has_const;
#endif
#if __has_attribute(nodiscard)
int has_nodiscard;
#endif

int use(void) { return imported + exported + bogus + scoped_bogus; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'dllimport' ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/unknown_attributes.c:2:16]
// WARN: 1 │
// WARN: 2 │ __attribute__((dllimport)) int imported;
// WARN: ·                ─────────
// WARN: 3 │ __attribute__((dllexport)) int exported = 1;
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'dllexport' ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/unknown_attributes.c:3:16]
// WARN: 2 │ __attribute__((dllimport)) int imported;
// WARN: 3 │ __attribute__((dllexport)) int exported = 1;
// WARN: ·                ─────────
// WARN: 4 │ __attribute__((noipa)) void opaque(void) {}
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'noipa' ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/unknown_attributes.c:4:16]
// WARN: 3 │ __attribute__((dllexport)) int exported = 1;
// WARN: 4 │ __attribute__((noipa)) void opaque(void) {}
// WARN: ·                ─────
// WARN: 5 │ __attribute__((overloadable)) void overloaded(int);
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'not_an_attribute' ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/unknown_attributes.c:7:16]
// WARN: 6 │ __attribute__((naked)) void bare(void);
// WARN: 7 │ __attribute__((not_an_attribute)) int bogus;
// WARN: ·                ────────────────
// WARN: 8 │ {{\[\[}}gnu::not_an_attribute]] int scoped_bogus;
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'gnu::not_an_attribute' ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/unknown_attributes.c:8:3]
// WARN: 7 │ __attribute__((not_an_attribute)) int bogus;
// WARN: 8 │ {{\[\[}}gnu::not_an_attribute]] int scoped_bogus;
// WARN: ·   ─────────────────────
// WARN: 9 │ {{\[\[}}clang::overloadable]] void clang_scoped(int);
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
// IR-WARN-NEXT:     global %[[VALUE_imported:[0-9]+]] imported: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_exported:[0-9]+]] exported: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_bogus:[0-9]+]] bogus: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_scoped_bogus:[0-9]+]] scoped_bogus: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_unterminated:[0-9]+]] unterminated: array<i8, 4> [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_has_naked:[0-9]+]] has_naked: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_has_nonstring:[0-9]+]] has_nonstring: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_has_const:[0-9]+]] has_const: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_opaque:[0-9]+]] @opaque() -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_overloaded:[0-9]+]] @overloaded(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_bare:[0-9]+]] @bare() -> void [linkage=external] [naked];
// IR-WARN-NEXT:     fn %[[VALUE_clang_scoped:[0-9]+]] @clang_scoped(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_msvc_scoped:[0-9]+]] @msvc_scoped() -> void [linkage=external] [inline=never];
// IR-WARN-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return add<i32>(add<i32>(add<i32>(read<i32>(%[[VALUE_imported]]), read<i32>(%[[VALUE_exported]])), read<i32>(%[[VALUE_bogus]])), read<i32>(%[[VALUE_scoped_bogus]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
