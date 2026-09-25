// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

#define C(a, b) __builtin_types_compatible_p(a, b)

struct T;
struct T *gt;
struct T;
static_assert(C(struct T *, typeof(gt)));

struct U { int a; };
struct U gu;

void hides(void) {
    struct T;
    static_assert(!C(struct T *, typeof(gt)));
    struct T;
    struct T *local;
    static_assert(C(struct T *, typeof(local)));
    struct T { long b; } completed;
    static_assert(C(struct T, typeof(completed)));
}

void hides_complete(void) {
    struct U;
    static_assert(!C(struct U *, typeof(&gu)));
    struct U { int a; } u;
    static_assert(sizeof(u) == sizeof(int));
}

void refers(void) {
    struct U *outer = &gu;
    static_assert(C(struct U *, typeof(outer)));
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
// IR-NEXT:     type @type0 T = struct incomplete;
// IR-NEXT:     type @type1 U = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type2 T = struct {
// IR-NEXT:         field0 b: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type3 U = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     global %1 gt: ptr<@type0> [storage=static] [linkage=external];
// IR-NEXT:     global %3 gu: @type1 [storage=static] [linkage=external];
// IR-NEXT:     fn %4 @hides() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %6 local: ptr<@type2> [storage=automatic];
// IR-NEXT:         let %7 completed: @type2 [storage=automatic];
// IR-NEXT:     }
// IR-NEXT:     fn %8 @hides_complete() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %10 u: @type3 [storage=automatic];
// IR-NEXT:     }
// IR-NEXT:     fn %11 @refers() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %12 outer: ptr<@type1> [storage=automatic] = addr_of<ptr<@type1>>(%3);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
