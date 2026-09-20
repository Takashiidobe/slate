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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Fwd = struct incomplete;
// IR-NEXT:     type @type1 Ret = struct incomplete;
// IR-NEXT:     type @type2 Completed = struct {
// IR-NEXT:         field0 y: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type3 Outer = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type4 Local = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type5 Local = struct {
// IR-NEXT:         field0 b: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type6 Outer = struct incomplete;
// IR-NEXT:     global %1 forward_pointer: ptr<@type0> [storage=static] [linkage=external];
// IR-NEXT:     global %6 completed_object: @type2 [storage=static] [linkage=external];
// IR-NEXT:     fn %3 @returns_incomplete() -> ptr<@type1> [linkage=external];
// IR-NEXT:     fn %8 @one() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %10 l: @type4 [storage=automatic];
// IR-NEXT:         write<i32>(field0(%10), const<i32>(4));
// IR-NEXT:         return read<i32>(field0(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @two() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %13 l: @type5 [storage=automatic];
// IR-NEXT:         write<i64>(field0(%13), widen<i64, reason=assign>(const<i32>(5)));
// IR-NEXT:         return truncate<i32, reason=explicit, fits=unknown>(read<i64>(field0(%13)));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @hides() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %16 inner: ptr<@type6> [storage=automatic];
// IR-NEXT:         read<ptr<@type6>>(%16);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
