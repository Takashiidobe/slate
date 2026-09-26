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

int use(void) { return imported + exported + bogus + scoped_bogus; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'dllimport' ignored
// WARN: ╭─[tests/fixtures/sema/unknown_attributes_clang_linux.c:2:16]
// WARN: 1 │
// WARN: 2 │ __attribute__((dllimport)) int imported;
// WARN: ·                ─────────
// WARN: 3 │ __attribute__((dllexport)) int exported = 1;
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'dllexport' ignored
// WARN: ╭─[tests/fixtures/sema/unknown_attributes_clang_linux.c:3:16]
// WARN: 2 │ __attribute__((dllimport)) int imported;
// WARN: 3 │ __attribute__((dllexport)) int exported = 1;
// WARN: ·                ─────────
// WARN: 4 │ __attribute__((noipa)) void opaque(void) {}
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'noipa' ignored
// WARN: ╭─[tests/fixtures/sema/unknown_attributes_clang_linux.c:4:16]
// WARN: 3 │ __attribute__((dllexport)) int exported = 1;
// WARN: 4 │ __attribute__((noipa)) void opaque(void) {}
// WARN: ·                ─────
// WARN: 5 │ __attribute__((overloadable)) void overloaded(int);
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'not_an_attribute' ignored
// WARN: ╭─[tests/fixtures/sema/unknown_attributes_clang_linux.c:7:16]
// WARN: 6 │ __attribute__((naked)) void bare(void);
// WARN: 7 │ __attribute__((not_an_attribute)) int bogus;
// WARN: ·                ────────────────
// WARN: 8 │ {{\[\[}}gnu::not_an_attribute]] int scoped_bogus;
// WARN: ╰────
// WARN: -Wunknown-attributes
// WARN: ⚠ unknown attribute 'gnu::not_an_attribute' ignored
// WARN: ╭─[tests/fixtures/sema/unknown_attributes_clang_linux.c:8:3]
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
// IR-WARN-NEXT:     global %0 imported: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %1 exported: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-WARN-NEXT:     global %5 bogus: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %6 scoped_bogus: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     fn %2 @opaque() -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %3 @overloaded(%10 <unnamed>: i32) -> void [linkage=external];
// IR-WARN-NEXT:     fn %4 @bare() -> void [linkage=external];
// IR-WARN-NEXT:     fn %7 @clang_scoped(%11 <unnamed>: i32) -> void [linkage=external];
// IR-WARN-NEXT:     fn %8 @msvc_scoped() -> void [linkage=external] [inline=never];
// IR-WARN-NEXT:     fn %9 @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return add<i32>(add<i32>(add<i32>(read<i32>(%0), read<i32>(%1)), read<i32>(%5)), read<i32>(%6));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
