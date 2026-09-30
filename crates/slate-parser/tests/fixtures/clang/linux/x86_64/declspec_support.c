// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

__declspec(dllimport) int imported;
__declspec(dllexport) int exported = 1;
__declspec(weak) int weak_value;
__declspec(visibility("hidden")) int hidden;
__declspec(aligned(16)) int gnu_aligned;
__declspec(mode(DI)) int moded;
__declspec(vector_size(16)) int vector;
__declspec(__noinline__) void wrapped(void);
__declspec(align(16)) int aligned;
__declspec(selectany) int selected = 1;

struct __declspec(packed) Packed {
  char c;
  int i;
};
int packed_size = sizeof(struct Packed);

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'dllimport' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:2:12]
// WARN: 1 │
// WARN: 2 │ __declspec(dllimport) int imported;
// WARN: ·            ─────────
// WARN: 3 │ __declspec(dllexport) int exported = 1;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'dllexport' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:3:12]
// WARN: 2 │ __declspec(dllimport) int imported;
// WARN: 3 │ __declspec(dllexport) int exported = 1;
// WARN: ·            ─────────
// WARN: 4 │ __declspec(weak) int weak_value;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'weak' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:4:12]
// WARN: 3 │ __declspec(dllexport) int exported = 1;
// WARN: 4 │ __declspec(weak) int weak_value;
// WARN: ·            ────
// WARN: 5 │ __declspec(visibility("hidden")) int hidden;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'visibility' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:5:12]
// WARN: 4 │ __declspec(weak) int weak_value;
// WARN: 5 │ __declspec(visibility("hidden")) int hidden;
// WARN: ·            ──────────
// WARN: 6 │ __declspec(aligned(16)) int gnu_aligned;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'aligned' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:6:12]
// WARN: 5 │ __declspec(visibility("hidden")) int hidden;
// WARN: 6 │ __declspec(aligned(16)) int gnu_aligned;
// WARN: ·            ───────
// WARN: 7 │ __declspec(mode(DI)) int moded;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'mode' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:7:12]
// WARN: 6 │ __declspec(aligned(16)) int gnu_aligned;
// WARN: 7 │ __declspec(mode(DI)) int moded;
// WARN: ·            ────
// WARN: 8 │ __declspec(vector_size(16)) int vector;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'vector_size' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:8:12]
// WARN: 7 │ __declspec(mode(DI)) int moded;
// WARN: 8 │ __declspec(vector_size(16)) int vector;
// WARN: ·            ───────────
// WARN: 9 │ __declspec(__noinline__) void wrapped(void);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute '__noinline__' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:9:12]
// WARN: 8 │ __declspec(vector_size(16)) int vector;
// WARN: 9 │ __declspec(__noinline__) void wrapped(void);
// WARN: ·            ────────────
// WARN: 10 │ __declspec(align(16)) int aligned;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'packed' is not supported
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/declspec_support.c:13:19]
// WARN: 12 │
// WARN: 13 │ struct __declspec(packed) Packed {
// WARN: ·                   ──────
// WARN: 14 │   char c;
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
// IR-WARN-NEXT:     type @type[[TYPE_Packed:[0-9]+]] Packed = struct {
// IR-WARN-NEXT:         field0 c: i8;
// IR-WARN-NEXT:         field1 i: i32;
// IR-WARN-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-WARN-NEXT:     global %[[VALUE_imported:[0-9]+]] imported: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_exported:[0-9]+]] exported: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_weak_value:[0-9]+]] weak_value: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_hidden:[0-9]+]] hidden: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_gnu_aligned:[0-9]+]] gnu_aligned: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_moded:[0-9]+]] moded: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_vector:[0-9]+]] vector: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_aligned:[0-9]+]] aligned: i32 [storage=static] [align=16] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_selected:[0-9]+]] selected: i32 [storage=static] = const<i32>(1) [linkage=external] [selectany];
// IR-WARN-NEXT:     global %[[VALUE_packed_size:[0-9]+]] packed_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_wrapped:[0-9]+]] @wrapped() -> void [linkage=external];
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
