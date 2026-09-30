// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct Fwd;
struct Fwd *forward_pointer;
struct Ret;
struct Ret *returns_incomplete(void);

struct Completed;
struct Completed { int y; };
struct Completed completed_object;

struct Outer { int x; };

struct Implicit *first_use;
struct Implicit { int z; };
struct Implicit implicit_object;

struct Holder { struct Member *m; };
struct Member { int w; };

int one(void) {
    struct Local { int a; } l;
    l.a = 4;
    return l.a;
}

int two(void) {
    struct Local { long b; } l;
    l.b = 5;
    return (int)l.b;
}

void hides(void) {
    struct Outer;
    struct Outer *inner;
    (void)inner;
}

void hides_implicitly(void) {
    struct Outer2 *inner;
    (void)inner;
}

struct Outer2 { int x; };
struct Outer2 outer2_object;

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
// IR-NEXT:     type @type[[TYPE_Fwd:[0-9]+]] Fwd = struct incomplete;
// IR-NEXT:     type @type[[TYPE_Ret:[0-9]+]] Ret = struct incomplete;
// IR-NEXT:     type @type[[TYPE_Completed:[0-9]+]] Completed = struct {
// IR-NEXT:         field0 y: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Outer:[0-9]+]] Outer = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Implicit:[0-9]+]] Implicit = struct {
// IR-NEXT:         field0 z: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Holder:[0-9]+]] Holder = struct {
// IR-NEXT:         field0 m: ptr<@type[[TYPE_Member:[0-9]+]]>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Member]] Member = struct {
// IR-NEXT:         field0 w: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Local:[0-9]+]] Local = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Local_2:[0-9]+]] Local = struct {
// IR-NEXT:         field0 b: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_Outer_2:[0-9]+]] Outer = struct incomplete;
// IR-NEXT:     type @type[[TYPE_Outer2:[0-9]+]] Outer2 = struct incomplete;
// IR-NEXT:     type @type[[TYPE_Outer2_2:[0-9]+]] Outer2 = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %[[VALUE_forward_pointer:[0-9]+]] forward_pointer: ptr<@type[[TYPE_Fwd]]> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_completed_object:[0-9]+]] completed_object: @type[[TYPE_Completed]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_first_use:[0-9]+]] first_use: ptr<@type[[TYPE_Implicit]]> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_implicit_object:[0-9]+]] implicit_object: @type[[TYPE_Implicit]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_outer2_object:[0-9]+]] outer2_object: @type[[TYPE_Outer2_2]] [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_returns_incomplete:[0-9]+]] @returns_incomplete() -> ptr<@type[[TYPE_Ret]]> [linkage=external];
// IR-NEXT:     fn %[[VALUE_one:[0-9]+]] @one() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_l:[0-9]+]] l: @type[[TYPE_Local]] [storage=automatic];
// IR-NEXT:         write<i32>(field0(%[[VALUE_l]]), const<i32>(4));
// IR-NEXT:         return read<i32>(field0(%[[VALUE_l]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_two:[0-9]+]] @two() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_l_2:[0-9]+]] l: @type[[TYPE_Local_2]] [storage=automatic];
// IR-NEXT:         write<i64>(field0(%[[VALUE_l_2]]), widen<i64, reason=assign>(const<i32>(5)));
// IR-NEXT:         return truncate<i32, reason=explicit, fits=unknown>(read<i64>(field0(%[[VALUE_l_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_hides:[0-9]+]] @hides() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_inner:[0-9]+]] inner: ptr<@type[[TYPE_Outer_2]]> [storage=automatic];
// IR-NEXT:         read<ptr<@type[[TYPE_Outer_2]]>>(%[[VALUE_inner]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_hides_implicitly:[0-9]+]] @hides_implicitly() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_inner_2:[0-9]+]] inner: ptr<@type[[TYPE_Outer2]]> [storage=automatic];
// IR-NEXT:         read<ptr<@type[[TYPE_Outer2]]>>(%[[VALUE_inner_2]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
