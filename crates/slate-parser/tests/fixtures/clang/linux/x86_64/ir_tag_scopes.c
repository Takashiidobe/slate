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
// IR-NEXT:     type @type0 Fwd = struct incomplete;
// IR-NEXT:     type @type1 Ret = struct incomplete;
// IR-NEXT:     type @type2 Completed = struct {
// IR-NEXT:         field0 y: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type3 Outer = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type4 Implicit = struct incomplete;
// IR-NEXT:     type @type5 Holder = struct {
// IR-NEXT:         field0 m: ptr<@type6>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type6 Member = struct {
// IR-NEXT:         field0 w: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type7 Local = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type8 Local = struct {
// IR-NEXT:         field0 b: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type9 Outer = struct incomplete;
// IR-NEXT:     type @type10 Outer2 = struct incomplete;
// IR-NEXT:     type @type11 Outer2 = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %1 forward_pointer: ptr<@type0> [storage=static] [linkage=external];
// IR-NEXT:     global %5 completed_object: @type2 [storage=static] [linkage=external];
// IR-NEXT:     global %8 first_use: ptr<@type4> [storage=static] [linkage=external];
// IR-NEXT:     global %9 implicit_object: @type4 [storage=static] [linkage=external];
// IR-NEXT:     global %25 outer2_object: @type11 [storage=static] [linkage=external];
// IR-NEXT:     fn %3 @returns_incomplete() -> ptr<@type1> [linkage=external];
// IR-NEXT:     fn %12 @one() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %14 l: @type7 [storage=automatic];
// IR-NEXT:         write<i32>(field0(%14), const<i32>(4));
// IR-NEXT:         return read<i32>(field0(%14));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @two() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %17 l: @type8 [storage=automatic];
// IR-NEXT:         write<i64>(field0(%17), widen<i64, reason=assign>(const<i32>(5)));
// IR-NEXT:         return truncate<i32, reason=explicit, fits=unknown>(read<i64>(field0(%17)));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @hides() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %20 inner: ptr<@type9> [storage=automatic];
// IR-NEXT:         read<ptr<@type9>>(%20);
// IR-NEXT:     }
// IR-NEXT:     fn %21 @hides_implicitly() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %23 inner: ptr<@type10> [storage=automatic];
// IR-NEXT:         read<ptr<@type10>>(%23);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
