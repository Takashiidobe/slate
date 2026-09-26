// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

extern __declspec(dllimport) int imported;
int *imported_address = &imported;
__declspec(dllexport) int exported = 1;
__declspec(weak) int weak_value;

struct __declspec(packed) Packed {
  char c;
  int i;
};
int packed_size = sizeof(struct Packed);

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ __declspec attribute 'weak' is not supported
// WARN: ╭─[tests/fixtures/sema/x86_64-pc-windows-msvc/declspec_support_clang.c:5:12]
// WARN: 4 │ __declspec(dllexport) int exported = 1;
// WARN: 5 │ __declspec(weak) int weak_value;
// WARN: ·            ────
// WARN: 6 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f64;
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
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     type @type0 Packed = struct {
// IR-WARN-NEXT:         field0 c: i8;
// IR-WARN-NEXT:         field1 i: i32;
// IR-WARN-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-WARN-NEXT:     extern %0 imported: i32 [storage=static] [linkage=external] [dllimport];
// IR-WARN-NEXT:     global %1 imported_address: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%0) [linkage=external];
// IR-WARN-NEXT:     global %2 exported: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// IR-WARN-NEXT:     global %3 weak_value: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %5 packed_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
