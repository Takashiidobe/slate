// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

struct s { char c; int n; } a, b;
struct outer { struct { long anonymous; }; struct s nested; } o;
int flag;

char member_of_conditional[sizeof((flag ? a : b).c) == 1 ? 1 : -1];
char member_of_anonymous[sizeof(o.anonymous) == sizeof(long) ? 1 : -1];
char member_through_pointer[sizeof((&o)->nested.n) == sizeof(int) ? 1 : -1];

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
// IR-NEXT:     type @type0 s = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 n: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 outer = struct {
// IR-NEXT:         field0 <anonymous>: @type2;
// IR-NEXT:         field1 nested: @type0;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type2 = struct {
// IR-NEXT:         field0 anonymous: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     global %1 a: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %2 b: @type0 [storage=static] [linkage=external];
// IR-NEXT:     global %5 o: @type1 [storage=static] [linkage=external];
// IR-NEXT:     global %6 flag: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %7 member_of_conditional: array<i8, 1> [storage=static] [linkage=external];
// IR-NEXT:     global %8 member_of_anonymous: array<i8, 1> [storage=static] [linkage=external];
// IR-NEXT:     global %9 member_through_pointer: array<i8, 1> [storage=static] [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
