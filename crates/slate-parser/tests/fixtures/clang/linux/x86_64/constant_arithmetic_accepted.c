// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

enum E {
    OVERFLOW = 2147483647 + 1,
    SHIFT_HIGH = 1 << 32,
    SHIFT_NEGATIVE = 1 << -1,
};

_Static_assert(OVERFLOW == (-2147483647 - 1), "");
_Static_assert(SHIFT_HIGH == (-2147483647 - 1), "");
_Static_assert(SHIFT_NEGATIVE == 0, "");

int overflow_initializer = 2147483647 + 1;
int selected_initializer = 1 ? 7 : 1 / 0;
int short_circuit_initializer = 0 && (1 / 0);
int unevaluated_initializer = sizeof(1 / 0);

#if (1 << -1) || (1 << 64)
#error invalid shifts must evaluate to zero
#endif

#if 1 << 63
int pp_shift_ok;
#endif

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
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : i32 {
// IR-NEXT:         %[[VALUE_OVERFLOW:[0-9]+]] OVERFLOW = const<i32>(-2147483648);
// IR-NEXT:         %[[VALUE_SHIFT_HIGH:[0-9]+]] SHIFT_HIGH = const<i32>(-2147483648);
// IR-NEXT:         %[[VALUE_SHIFT_NEGATIVE:[0-9]+]] SHIFT_NEGATIVE = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %[[VALUE_overflow_initializer:[0-9]+]] overflow_initializer: i32 [storage=static] = add<i32>(const<i32>(2147483647), const<i32>(1)) [linkage=external];
// IR-NEXT:     global %[[VALUE_selected_initializer:[0-9]+]] selected_initializer: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(7), div<i32>(const<i32>(1), const<i32>(0))) [linkage=external];
// IR-NEXT:     global %[[VALUE_short_circuit_initializer:[0-9]+]] short_circuit_initializer: i32 [storage=static] = from_bool<i32>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(div<i32>(const<i32>(1), const<i32>(0)), const<i32>(0)))) [linkage=external];
// IR-NEXT:     global %[[VALUE_unevaluated_initializer:[0-9]+]] unevaluated_initializer: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_pp_shift_ok:[0-9]+]] pp_shift_ok: i32 [storage=static] [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
