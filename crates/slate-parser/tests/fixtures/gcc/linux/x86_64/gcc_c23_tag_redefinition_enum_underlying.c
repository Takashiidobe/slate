// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES DIRECT DIRECT
// SLATE-FILECHECK-DEFINES POINTER POINTER
// SLATE-FILECHECK-IR-ERROR DIRECT
// SLATE-FILECHECK-IR-ERROR POINTER
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

enum E { A };

typedef int I;
struct Typedef { I x; };
struct Typedef { int x; };

struct Typedef typedefed;

#ifdef DIRECT
struct Direct { enum E x; };
struct Direct { unsigned x; };
#endif

#ifdef POINTER
struct Pointer { enum E *p; };
struct Pointer { unsigned *p; };
#endif

// SLATE-FILECHECK-BEGIN DIRECT
// DIRECT: Error:   × semantic analysis failed
// DIRECT: Error:
// DIRECT: × invalid in this context: redefinition of struct, union, or enum tag
// DIRECT: ╭─[tests/fixtures/gcc/linux/x86_64/gcc_c23_tag_redefinition_enum_underlying.c:12:1]
// DIRECT: 11 │ struct Direct { enum E x; };
// DIRECT: 12 │ struct Direct { unsigned x; };
// DIRECT: · ──────────────────────────────
// DIRECT: 13 │ #endif
// DIRECT: ╰────
// SLATE-FILECHECK-END DIRECT
// SLATE-FILECHECK-BEGIN POINTER
// POINTER: Error:   × semantic analysis failed
// POINTER: Error:
// POINTER: × invalid in this context: redefinition of struct, union, or enum tag
// POINTER: ╭─[tests/fixtures/gcc/linux/x86_64/gcc_c23_tag_redefinition_enum_underlying.c:17:1]
// POINTER: 16 │ struct Pointer { enum E *p; };
// POINTER: 17 │ struct Pointer { unsigned *p; };
// POINTER: · ────────────────────────────────
// POINTER: 18 │ #endif
// POINTER: ╰────
// SLATE-FILECHECK-END POINTER
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     type @type0 E = enum : u32 {
// VALID-NEXT:         %0 A = const<i32>(0);
// VALID-NEXT:     } [size=4, align=4];
// VALID-NEXT:     type @type1 I = i32;
// VALID-NEXT:     type @type2 Typedef = struct {
// VALID-NEXT:         field0 x: i32;
// VALID-NEXT:     } [size=4, align=4, offsets=[0]];
// VALID-NEXT:     global %4 typedefed: @type2 [storage=static] [linkage=external];
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
