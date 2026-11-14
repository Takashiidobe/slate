// SLATE-FILECHECK-DEFINES DEFAULT
static int abs_value = __builtin_abs(-3);
static long labs_value = __builtin_labs(-40L);
static long long llabs_value = __builtin_llabs(-500LL);
static long expected = __builtin_expect(7, 1);
static long expected_probability = __builtin_expect_with_probability(8, 1, 0.9);
static unsigned long length = __builtin_strlen("kernel");
static int ordered = __builtin_strcmp("abc", "abd");
static int same = __builtin_memcmp("abc", "abd", 2);
static double magnitude = __builtin_fabs(-2.5);
static float magnitude_float = __builtin_fabsf(-1.25f);
static double signed_copy = __builtin_copysign(3.0, -0.0);
static double larger = __builtin_fmax(1.5, -7.0);
static double smaller = __builtin_fmin(-1.5, 4.0);
static unsigned long lengths[] = {__builtin_strlen("a"), __builtin_strlen("")};

int runtime_abs(int x) {
    int automatic = __builtin_abs(-9);
    return automatic + __builtin_abs(x);
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_abs_value:[0-9]+]] abs_value: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_labs_value:[0-9]+]] labs_value: i64 [storage=static] = const<i64>(40) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_llabs_value:[0-9]+]] llabs_value: i64 [storage=static] = const<i64>(500) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: i64 [storage=static] = const<i64>(7) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_expected_probability:[0-9]+]] expected_probability: i64 [storage=static] = const<i64>(8) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_length:[0-9]+]] length: u64 [storage=static] = reinterpret<u64, reason=assign>(const<u64>(6) [c="__size_t"]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ordered:[0-9]+]] ordered: i32 [storage=static] = const<i32>(-1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_same:[0-9]+]] same: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_magnitude:[0-9]+]] magnitude: f64 [storage=static] = const<f64>(2.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_magnitude_float:[0-9]+]] magnitude_float: f32 [storage=static] = const<f32>(1.25) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_signed_copy:[0-9]+]] signed_copy: f64 [storage=static] = const<f64>(-3.0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_larger:[0-9]+]] larger: f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_smaller:[0-9]+]] smaller: f64 [storage=static] = const<f64>(-1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_lengths:[0-9]+]] lengths: array<u64, 2> [storage=static] [align=16] = aggregate<array<u64, 2>, zero_fill=false>(index0 = reinterpret<u64, reason=assign>(const<u64>(1) [c="__size_t"]), index1 = reinterpret<u64, reason=assign>(const<u64>(0) [c="__size_t"])) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abs:[0-9]+]] @__builtin_abs(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_runtime_abs:[0-9]+]] @runtime_abs(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_automatic:[0-9]+]] automatic: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs]], neg<i32, overflow=ub>(const<i32>(9)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_automatic]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs]], read<i32>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
