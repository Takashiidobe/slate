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
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_N:[0-9]+]] N = struct {
// IR-NEXT:         field0 next: ptr<@type[[TYPE_N]]>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// IR-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_W:[0-9]+]] W = struct {
// IR-NEXT:         field0 x: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_O:[0-9]+]] O = struct {
// IR-NEXT:         field0 s: @type[[TYPE_S]];
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_S_2:[0-9]+]] S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_N_2:[0-9]+]] N = struct {
// IR-NEXT:         field0 next: ptr<@type[[TYPE_N_2]]>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_U_2:[0-9]+]] U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_E_2:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_A]] A = const<i32>(0);
// IR-NEXT:         %[[VALUE_B]] B = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_W_2:[0-9]+]] W = struct {
// IR-NEXT:         field0 x: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_O_2:[0-9]+]] O = struct {
// IR-NEXT:         field0 s: @type[[TYPE_S_2]];
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_S_3:[0-9]+]] S = struct {
// IR-NEXT:         field0 y: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_W_3:[0-9]+]] W = struct {
// IR-NEXT:         field0 x: i32 : 4;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type[[TYPE_N_3:[0-9]+]] N = struct {
// IR-NEXT:         field0 next: ptr<@type[[TYPE_N_3]]>;
// IR-NEXT:         field1 z: i32;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_E_3:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_A]] A = const<i32>(1);
// IR-NEXT:         %[[VALUE_B]] B = const<i32>(2);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_O_3:[0-9]+]] O = union {
// IR-NEXT:         field0 s: @type[[TYPE_S_3]];
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_S_4:[0-9]+]] S = struct {
// IR-NEXT:         field0 x: const i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_O_4:[0-9]+]] O = struct {
// IR-NEXT:         field0 s: @type[[TYPE_S_4]];
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_S_5:[0-9]+]] S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// IR-NEXT:         field0 s: @type[[TYPE_S_5]];
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %[[VALUE_B]] gs: @type[[TYPE_S]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_gn:[0-9]+]] gn: @type[[TYPE_N]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_gu:[0-9]+]] gu: @type[[TYPE_U]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_ge:[0-9]+]] ge: @type[[TYPE_E]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_gw:[0-9]+]] gw: @type[[TYPE_W]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_go:[0-9]+]] go: @type[[TYPE_O]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_ga:[0-9]+]] ga: @type[[TYPE0]] [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_compatible:[0-9]+]] @compatible() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_la:[0-9]+]] la: @type[[TYPE1]] [storage=automatic];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_incompatible:[0-9]+]] @incompatible() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_qualified:[0-9]+]] @qualified() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_initialize:[0-9]+]] @initialize() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_whole:[0-9]+]] whole: @type[[TYPE_S_5]] [storage=automatic] = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]]));
// IR-NEXT:         let %[[VALUE_pointer:[0-9]+]] pointer: ptr<@type[[TYPE_S_5]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_S_5]]>, reason=assign>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_B]]));
// IR-NEXT:         let %[[VALUE_elided:[0-9]+]] elided: array<@type[[TYPE_S_5]], 2> [storage=automatic] = aggregate<array<@type[[TYPE_S_5]], 2>, zero_fill=false>(index0 = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]])), index1 = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]])));
// IR-NEXT:         let %[[VALUE_inferred:[0-9]+]] inferred: array<@type[[TYPE_S_5]], 3> [storage=automatic] = aggregate<array<@type[[TYPE_S_5]], 3>, zero_fill=false>(index0 = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]])), index1 = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]])), index2 = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]])));
// IR-NEXT:         let %[[VALUE_member:[0-9]+]] member: @type[[TYPE_P]] [storage=automatic] = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = copy<@type[[TYPE_S_5]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_B]])), field1 = const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
