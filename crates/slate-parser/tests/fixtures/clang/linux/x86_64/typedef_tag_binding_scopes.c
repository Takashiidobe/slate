// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef int T;

void f(void) {
    typedef long T;
    T x;
    _Static_assert(sizeof x == 8, "");
    {
        struct T { char c; };
    }
}

T y;
_Static_assert(sizeof y == 4, "");

struct T { int a[3]; };

void g(void) {
    struct T;
    struct T *p;
    {
        struct T { char c; } q;
        _Static_assert(sizeof q == 1, "");
    }
}

_Static_assert(sizeof(struct T) == 12, "");

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
// IR-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// IR-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = i64;
// IR-NEXT:     type @type[[TYPE_T_3:[0-9]+]] T = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_T_4:[0-9]+]] T = struct {
// IR-NEXT:         field0 a: array<i32, 3>;
// IR-NEXT:     } [size=12, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_T_5:[0-9]+]] T = struct incomplete;
// IR-NEXT:     type @type[[TYPE_T_6:[0-9]+]] T = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     global %[[VALUE_y:[0-9]+]] y: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_x:[0-9]+]] x: i64 [storage=automatic];
// IR-NEXT:         {
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_T_5]]> [storage=automatic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_q:[0-9]+]] q: @type[[TYPE_T_6]] [storage=automatic];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
