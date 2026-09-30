long long file_scope;
unsigned long long unsigned_file_scope;
typedef long long ll_t;
struct S {
  long long field;
};
long long returns_long_long(long long parameter);
void body(void) { long long local; }
ll_t typedef_use_does_not_warn;

// SLATE-FILECHECK-ARGS -pedantic
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-WARNING C89

// SLATE-FILECHECK-BEGIN C89
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:1:1]
// C89: 1 │ long long file_scope;
// C89: · ─────────────────────
// C89: 2 │ unsigned long long unsigned_file_scope;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:2:1]
// C89: 1 │ long long file_scope;
// C89: 2 │ unsigned long long unsigned_file_scope;
// C89: · ───────────────────────────────────────
// C89: 3 │ typedef long long ll_t;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:3:1]
// C89: 2 │ unsigned long long unsigned_file_scope;
// C89: 3 │ typedef long long ll_t;
// C89: · ───────────────────────
// C89: 4 │ struct S {
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:5:3]
// C89: 4 │ struct S {
// C89: 5 │   long long field;
// C89: ·   ────────────────
// C89: 6 │ };
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:7:1]
// C89: 6 │ };
// C89: 7 │ long long returns_long_long(long long parameter);
// C89: · ─────────────────────────────────────────────────
// C89: 8 │ void body(void) { long long local; }
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:7:1]
// C89: 6 │ };
// C89: 7 │ long long returns_long_long(long long parameter);
// C89: · ─────────────────────────────────────────────────
// C89: 8 │ void body(void) { long long local; }
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/clang/linux/x86_64/long_long_declaration_warnings.c:8:19]
// C89: 7 │ long long returns_long_long(long long parameter);
// C89: 8 │ void body(void) { long long local; }
// C89: ·                   ────────────────
// C89: 9 │ ll_t typedef_use_does_not_warn;
// C89: ╰────
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN IR-C89
// IR-C89: module {
// IR-C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C89-NEXT:         endian = little;
// IR-C89-NEXT:         pointer [size=8, align=8];
// IR-C89-NEXT:         stack_alignment = 16;
// IR-C89-NEXT:         long_double = f80;
// IR-C89-NEXT:         storage bool [size=1, align=1];
// IR-C89-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C89-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C89-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C89-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C89-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C89-NEXT:         storage bf16 [size=2, align=2];
// IR-C89-NEXT:         storage f16 [size=2, align=2];
// IR-C89-NEXT:         storage f32 [size=4, align=4];
// IR-C89-NEXT:         storage f64 [size=8, align=8];
// IR-C89-NEXT:         storage f80 [size=16, align=16];
// IR-C89-NEXT:         storage f128 [size=16, align=16];
// IR-C89-NEXT:         storage d32 [size=4, align=4];
// IR-C89-NEXT:         storage d64 [size=8, align=8];
// IR-C89-NEXT:         storage d128 [size=16, align=16];
// IR-C89-NEXT:     }
// IR-C89-NEXT:     type @type[[TYPE_ll_t:[0-9]+]] ll_t = i64;
// IR-C89-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-C89-NEXT:         field0 field: i64;
// IR-C89-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-C89-NEXT:     global %[[VALUE_file_scope:[0-9]+]] file_scope: i64 [storage=static] [linkage=external];
// IR-C89-NEXT:     global %[[VALUE_unsigned_file_scope:[0-9]+]] unsigned_file_scope: u64 [storage=static] [linkage=external];
// IR-C89-NEXT:     global %[[VALUE_typedef_use_does_not_warn:[0-9]+]] typedef_use_does_not_warn: i64 [storage=static] [linkage=external];
// IR-C89-NEXT:     fn %[[VALUE_returns_long_long:[0-9]+]] @returns_long_long(%[[VALUE_parameter:[0-9]+]] parameter: i64) -> i64 [linkage=external];
// IR-C89-NEXT:     fn %[[VALUE_body:[0-9]+]] @body() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C89-NEXT:         let %[[VALUE_local:[0-9]+]] local: i64 [storage=automatic];
// IR-C89-NEXT:     }
// IR-C89-NEXT: }
// SLATE-FILECHECK-END IR-C89
