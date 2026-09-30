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
// IR-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 n: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// IR-NEXT:         field1 nested: @type[[TYPE_s]];
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 anonymous: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_s]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_s]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_o:[0-9]+]] o: @type[[TYPE_outer]] [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_member_of_conditional:[0-9]+]] member_of_conditional: array<i8, 1> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_member_of_anonymous:[0-9]+]] member_of_anonymous: array<i8, 1> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_member_through_pointer:[0-9]+]] member_through_pointer: array<i8, 1> [storage=static] [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
