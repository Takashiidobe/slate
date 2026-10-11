// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct hfa {
  float a;
  float b;
};

long __attribute__((ms_abi)) ms_fixed(int a, double b, struct hfa h);
long __attribute__((ms_abi)) ms_variadic(int n, ...);
long __attribute__((sysv_abi)) sysv_ignored(long a);

long use(struct hfa h) {
  return ms_fixed(1, 2.0, h) + ms_variadic(2, 1.5, h) + sysv_ignored(1);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_hfa:[0-9]+]] hfa = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_ms_fixed:[0-9]+]] @ms_fixed(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_h:[0-9]+]] h: @type[[TYPE_hfa]]) -> i64 [linkage=external] [abi=win_arm64 ms_abi(scalar, scalar, native_c) -> scalar];
// IR-NEXT:     fn %[[VALUE_ms_variadic:[0-9]+]] @ms_variadic(%[[VALUE_n:[0-9]+]] n: i32, ...) -> i64 [linkage=external] [abi=win_arm64 ms_abi(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_sysv_ignored:[0-9]+]] @sysv_ignored(%[[VALUE_a_2:[0-9]+]] a: i64) -> i64 [linkage=external];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_h_2:[0-9]+]] h: @type[[TYPE_hfa]]) -> i64 [linkage=external] [abi=aapcs64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i64>(add<i64>(call<i64, abi=win_arm64 ms_abi(scalar, scalar, native_c) -> scalar>(%[[VALUE_ms_fixed]], const<i32>(1), const<f64>(2.0), copy<@type[[TYPE_hfa]], reason=arg>(read<@type[[TYPE_hfa]]>(%[[VALUE_h_2]]))), call<i64, abi=win_arm64 ms_abi(scalar, scalar, coerce<i64>) -> scalar>(%[[VALUE_ms_variadic]], const<i32>(2), const<f64>(1.5), copy<@type[[TYPE_hfa]], reason=vararg>(read<@type[[TYPE_hfa]]>(%[[VALUE_h_2]])))), call<i64>(%[[VALUE_sysv_ignored]], widen<i64>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
