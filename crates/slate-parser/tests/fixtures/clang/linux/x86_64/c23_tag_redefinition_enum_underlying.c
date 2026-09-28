// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES INCOMPLETE_ARRAY INCOMPLETE_ARRAY
// SLATE-FILECHECK-DEFINES QUALIFIED QUALIFIED
// SLATE-FILECHECK-IR-ERROR INCOMPLETE_ARRAY
// SLATE-FILECHECK-IR-ERROR QUALIFIED
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

enum E { A };
enum L : long { B };

struct Direct { enum E x; };
struct Direct { unsigned x; };

struct Pointer { enum E *p; };
struct Pointer { unsigned *p; };

struct Fixed { enum L x; };
struct Fixed { long x; };

struct Parameter { void (*f)(enum E); };
struct Parameter { void (*f)(unsigned); };

struct Array { enum E a[2]; };
struct Array { unsigned a[2]; };

typedef int I;
struct Typedef { I x; };
struct Typedef { int x; };

struct Direct direct;
struct Pointer pointer;
struct Fixed fixed;
struct Parameter parameter;
struct Array array;
struct Typedef typedefed;

#ifdef INCOMPLETE_ARRAY
struct Extent { int (*p)[]; };
struct Extent { int (*p)[3]; };
#endif

#ifdef QUALIFIED
struct Const { const enum E x; };
struct Const { unsigned x; };
#endif

// SLATE-FILECHECK-BEGIN INCOMPLETE_ARRAY
// INCOMPLETE_ARRAY: Error:   × semantic analysis failed
// INCOMPLETE_ARRAY: Error:
// INCOMPLETE_ARRAY: × redefinition of struct, union, or enum tag
// INCOMPLETE_ARRAY: ╭─[tests/fixtures/clang/linux/x86_64/c23_tag_redefinition_enum_underlying.c:33:1]
// INCOMPLETE_ARRAY: 32 │ struct Extent { int (*p)[]; };
// INCOMPLETE_ARRAY: 33 │ struct Extent { int (*p)[3]; };
// INCOMPLETE_ARRAY: · ───────────────────────────────
// INCOMPLETE_ARRAY: 34 │ #endif
// INCOMPLETE_ARRAY: ╰────
// SLATE-FILECHECK-END INCOMPLETE_ARRAY
// SLATE-FILECHECK-BEGIN QUALIFIED
// QUALIFIED: Error:   × semantic analysis failed
// QUALIFIED: Error:
// QUALIFIED: × redefinition of struct, union, or enum tag
// QUALIFIED: ╭─[tests/fixtures/clang/linux/x86_64/c23_tag_redefinition_enum_underlying.c:38:1]
// QUALIFIED: 37 │ struct Const { const enum E x; };
// QUALIFIED: 38 │ struct Const { unsigned x; };
// QUALIFIED: · ─────────────────────────────
// QUALIFIED: 39 │ #endif
// QUALIFIED: ╰────
// SLATE-FILECHECK-END QUALIFIED
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
// VALID-NEXT:     type @type1 L = enum : i64 {
// VALID-NEXT:         %0 B = const<@type1>(0);
// VALID-NEXT:     } [size=8, align=8];
// VALID-NEXT:     type @type2 Direct = struct {
// VALID-NEXT:         field0 x: @type0;
// VALID-NEXT:     } [size=4, align=4, offsets=[0]];
// VALID-NEXT:     type @type3 Pointer = struct {
// VALID-NEXT:         field0 p: ptr<@type0>;
// VALID-NEXT:     } [size=8, align=8, offsets=[0]];
// VALID-NEXT:     type @type4 Fixed = struct {
// VALID-NEXT:         field0 x: @type1;
// VALID-NEXT:     } [size=8, align=8, offsets=[0]];
// VALID-NEXT:     type @type5 Parameter = struct {
// VALID-NEXT:         field0 f: ptr<fn(@type0) -> void>;
// VALID-NEXT:     } [size=8, align=8, offsets=[0]];
// VALID-NEXT:     type @type6 Array = struct {
// VALID-NEXT:         field0 a: array<@type0, 2>;
// VALID-NEXT:     } [size=8, align=4, offsets=[0]];
// VALID-NEXT:     type @type7 I = i32;
// VALID-NEXT:     type @type8 Typedef = struct {
// VALID-NEXT:         field0 x: i32;
// VALID-NEXT:     } [size=4, align=4, offsets=[0]];
// VALID-NEXT:     global %11 direct: @type2 [storage=static] [linkage=external];
// VALID-NEXT:     global %12 pointer: @type3 [storage=static] [linkage=external];
// VALID-NEXT:     global %13 fixed: @type4 [storage=static] [linkage=external];
// VALID-NEXT:     global %14 parameter: @type5 [storage=static] [linkage=external];
// VALID-NEXT:     global %15 array: @type6 [storage=static] [linkage=external];
// VALID-NEXT:     global %16 typedefed: @type8 [storage=static] [linkage=external];
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
