// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef int (__attribute__((regcall)) *callback_t)(int);

int __regcall transform(int x);
int transform(int x) { return x + 1; }

_Static_assert(__builtin_types_compatible_p(__typeof__(&transform), callback_t), "");
_Static_assert(!__builtin_types_compatible_p(callback_t, int (*)(int)), "");

int invoke(callback_t callback) {
  return transform(1) + callback(2);
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
// IR-NEXT:     type @type[[TYPE_callback_t:[0-9]+]] callback_t = ptr<fn regcall(i32) -> i32>;
// IR-NEXT:     fn %[[VALUE_transform:[0-9]+]] @transform(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [abi=sysv64 regcall(scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_invoke:[0-9]+]] @invoke(%[[VALUE_callback:[0-9]+]] callback: ptr<fn regcall(i32) -> i32> [c="callback_t"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(callback_t)"] {
// IR-NEXT:         return add<i32>(call<i32, abi=sysv64 regcall(scalar) -> scalar>(%[[VALUE_transform]], const<i32>(1)), call<i32, abi=sysv64 regcall(scalar) -> scalar>(read<ptr<fn regcall(i32) -> i32>>(%[[VALUE_callback]]) [c="callback_t"], const<i32>(2)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
