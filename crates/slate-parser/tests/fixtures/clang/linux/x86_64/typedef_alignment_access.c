// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef long long V __attribute__((vector_size(16)));
typedef struct S { V b; } P __attribute__((aligned(1)));
typedef int A __attribute__((aligned(1)));
V f(P *p) { return p->b; }
int g(A *a) { return *a; }

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
// IR-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<i64, 2>;
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 b: vector<i64, 2>;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = @type[[TYPE_S]];
// IR-NEXT:     type @type[[TYPE_A:[0-9]+]] A = i32;
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> vector<i64, 2> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i64, 2>, align=1>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a:[0-9]+]] a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32, align=1>(deref(read<ptr<i32>>(%[[VALUE_a]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
