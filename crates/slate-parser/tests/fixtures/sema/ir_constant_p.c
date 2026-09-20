// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

enum { K = 3 };
int g;
volatile int v;

_Static_assert(__builtin_constant_p(1), "");
_Static_assert(__builtin_constant_p(K * 2 + sizeof(int)), "");
_Static_assert(__builtin_constant_p(1.5), "");
_Static_assert(!__builtin_constant_p(g), "");
_Static_assert(!__builtin_constant_p(v), "");
_Static_assert(__builtin_types_compatible_p(int, signed), "");

int by_enum[K];
int bounded[__builtin_constant_p(K) ? 4 : 8];
int unbounded[__builtin_constant_p(g) ? 4 : 8];

int f(int x) {
  return __builtin_constant_p(K) + __builtin_constant_p(x) + __builtin_constant_p(g++);
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
// IR-NEXT:     type @type0 = enum : u32 {
// IR-NEXT:         %0 K = const<i32>(3);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %2 g: i32 [storage=static] [linkage=external] [c="int"];
// IR-NEXT:     global %3 v: volatile i32 [storage=static] [linkage=external] [c="volatile int"] [c_volatile="true"];
// IR-NEXT:     global %4 by_enum: array<i32, 3> [storage=static] [linkage=external] [c="int[3]"];
// IR-NEXT:     global %5 bounded: array<i32, 4> [storage=static] [linkage=external] [c="int[4]"];
// IR-NEXT:     global %6 unbounded: array<i32, 8> [storage=static] [linkage=external] [c="int[8]"];
// IR-NEXT:     fn %7 @f(%8 x: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1) [c_builtin="__builtin_constant_p"], const<i32>(0) [c_builtin="__builtin_constant_p"]), const<i32>(0) [c_builtin="__builtin_constant_p"]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
