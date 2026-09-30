// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

typedef int T;
typedef const int CT;
enum E { A };
struct S { int x; };

_Static_assert(__builtin_types_compatible_p(int, T), "");
_Static_assert(!__builtin_types_compatible_p(long, long long), "");

int f(void) {
  return __builtin_types_compatible_p(int, T)
       + __builtin_types_compatible_p(const int, CT)
       + __builtin_types_compatible_p(volatile int, int)
       + __builtin_types_compatible_p(int *const, int *)
       + __builtin_types_compatible_p(const int *, int *)
       + __builtin_types_compatible_p(long, long long)
       + __builtin_types_compatible_p(char, signed char)
       + __builtin_types_compatible_p(enum E, unsigned int)
       + __builtin_types_compatible_p(struct S, struct S)
       + __builtin_types_compatible_p(int (*const)(int *), int (*)(int *))
       + __builtin_types_compatible_p(const int[3], int[3]);
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
// IR-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32 [c="int"];
// IR-NEXT:     type @type[[TYPE_CT:[0-9]+]] CT = i32 [c="const int"] [c_const="true"];
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1) [types_compatible="int, int"], const<i32>(1) [types_compatible="int, int"]), const<i32>(1) [types_compatible="int, int"]), const<i32>(1) [types_compatible="int *, int *"]), const<i32>(0) [types_compatible="const int *, int *"]), const<i32>(0) [types_compatible="long, long long"]), const<i32>(0) [types_compatible="char, signed char"]), const<i32>(1) [types_compatible="enum E, unsigned int"]), const<i32>(1) [types_compatible="struct S, struct S"]), const<i32>(1) [types_compatible="int (*)(int *), int (*)(int *)"]), const<i32>(1) [types_compatible="int[3], int[3]"]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
