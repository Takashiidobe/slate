// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

#define C(a, b) __builtin_types_compatible_p(a, b)

struct S { int x; };            struct S gs;
struct N { struct N *next; };   struct N gn;
union U { int i; float f; };    union U gu;
enum E { A, B };                enum E ge;
struct W { int x : 3; };        struct W gw;
struct O { struct S s; };       struct O go;
struct { int x; } ga;

void compatible(void) {
    struct S { int x; };           static_assert(C(struct S, typeof(gs)));
    struct N { struct N *next; };  static_assert(C(struct N, typeof(gn)));
    union U { int i; float f; };   static_assert(C(union U, typeof(gu)));
    enum E { A, B };               static_assert(C(enum E, typeof(ge)));
    struct W { int x : 3; };       static_assert(C(struct W, typeof(gw)));
    struct O { struct S s; };      static_assert(C(struct O, typeof(go)));
    struct { int x; } la;          static_assert(!C(typeof(la), typeof(ga)));
}

void incompatible(void) {
    struct S { int y; };                  static_assert(!C(struct S, typeof(gs)));
    struct W { int x : 4; };              static_assert(!C(struct W, typeof(gw)));
    struct N { struct N *next; int z; };  static_assert(!C(struct N, typeof(gn)));
    enum E { A = 1, B };                  static_assert(!C(enum E, typeof(ge)));
    union O { struct S s; };              static_assert(!C(union O, typeof(go)));
}

void qualified(void) {
    struct S { const int x; };  static_assert(!C(struct S, typeof(gs)));
    struct O { struct S s; };   static_assert(!C(struct O, typeof(go)));
}

void initialize(void) {
    struct S { int x; };
    struct S whole = gs;
    struct S *pointer = &gs;
    struct S elided[2] = {gs, gs};
    struct S inferred[] = {gs, gs, gs};
    static_assert(sizeof(inferred) / sizeof(inferred[0]) == 3);
    struct P { struct S s; int y; } member = {gs, 1};
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type1 N = struct {
// IR-NEXT:         field0 next: ptr<@type1>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type2 U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type3 E = enum : u32 {
// IR-NEXT:         %0 A = const<i32>(0);
// IR-NEXT:         %1 B = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type4 W = struct {
// IR-NEXT:         field0 x: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type5 O = struct {
// IR-NEXT:         field0 s: @type0;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type6 = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type7 S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type8 N = struct {
// IR-NEXT:         field0 next: ptr<@type8>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type9 U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type10 E = enum : u32 {
// IR-NEXT:         %0 A = const<i32>(0);
// IR-NEXT:         %1 B = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type11 W = struct {
// IR-NEXT:         field0 x: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type12 O = struct {
// IR-NEXT:         field0 s: @type7;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type13 = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type14 S = struct {
// IR-NEXT:         field0 y: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type15 W = struct {
// IR-NEXT:         field0 x: i32 : 4;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type16 N = struct {
// IR-NEXT:         field0 next: ptr<@type16>;
// IR-NEXT:         field1 z: i32;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type17 E = enum : u32 {
// IR-NEXT:         %0 A = const<i32>(1);
// IR-NEXT:         %1 B = const<i32>(2);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type18 O = union {
// IR-NEXT:         field0 s: @type14;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type19 S = struct {
// IR-NEXT:         field0 x: const i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type20 O = struct {
// IR-NEXT:         field0 s: @type19;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type21 S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type22 P = struct {
// IR-NEXT:         field0 s: @type21;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %1 gs: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %3 gn: @type1 [storage=static] [linkage=external];
// IR-NEXT:     global %5 gu: @type2 [storage=static] [linkage=external];
// IR-NEXT:     global %9 ge: @type3 [storage=static] [linkage=external];
// IR-NEXT:     global %11 gw: @type4 [storage=static] [linkage=external];
// IR-NEXT:     global %13 go: @type5 [storage=static] [linkage=external];
// IR-NEXT:     global %15 ga: @type6 [storage=static] [linkage=external];
// IR-NEXT:     fn %16 @compatible() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %26 la: @type13 [storage=automatic];
// IR-NEXT:     }
// IR-NEXT:     fn %27 @incompatible() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %35 @qualified() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %38 @initialize() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %40 whole: @type21 [storage=automatic] = copy<@type21, reason=assign>(read<@type0>(%1));
// IR-NEXT:         let %41 pointer: ptr<@type21> [storage=automatic] = pointer_cast<ptr<@type21>, reason=assign>(addr_of<ptr<@type0>>(%1));
// IR-NEXT:         let %42 elided: array<@type21, 2> [storage=automatic] = aggregate<array<@type21, 2>, zero_fill=false>(index0 = copy<@type21, reason=assign>(read<@type0>(%1)), index1 = copy<@type21, reason=assign>(read<@type0>(%1)));
// IR-NEXT:         let %43 inferred: array<@type21, 3> [storage=automatic] = aggregate<array<@type21, 3>, zero_fill=false>(index0 = copy<@type21, reason=assign>(read<@type0>(%1)), index1 = copy<@type21, reason=assign>(read<@type0>(%1)), index2 = copy<@type21, reason=assign>(read<@type0>(%1)));
// IR-NEXT:         let %45 member: @type22 [storage=automatic] = aggregate<@type22, zero_fill=false>(field0 = copy<@type21, reason=assign>(read<@type0>(%1)), field1 = const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
