// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

typedef long long(__attribute__((sysv_abi)) *sysv_callback)(long long, double);

long long __attribute__((sysv_abi)) sysv_fixed(int a, double b, long long c);
long long __attribute__((sysv_abi)) sysv_variadic(int n, ...);
long long __attribute__((ms_abi)) ms_native(long long a);

long long __attribute__((sysv_abi)) sysv_defined(long long a, double b) {
  return a + (long long)b;
}

long long use(sysv_callback cb) {
  sysv_callback local = sysv_defined;
  return sysv_fixed(1, 2.0, 3) + sysv_variadic(2, 1LL, 2.5) + ms_native(1) + cb(1, 2.0) +
         local(3, 4.0);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:     type @type[[TYPE_sysv_callback:[0-9]+]] sysv_callback = ptr<fn sysv_abi(i64, f64) -> i64>;
// IR-NEXT:     fn %[[VALUE_sysv_fixed:[0-9]+]] @sysv_fixed(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: i64) -> i64 [linkage=external] [abi=sysv64 sysv_abi(scalar, scalar, scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_sysv_variadic:[0-9]+]] @sysv_variadic(%[[VALUE_n:[0-9]+]] n: i32, ...) -> i64 [linkage=external] [abi=sysv64 sysv_abi(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_ms_native:[0-9]+]] @ms_native(%[[VALUE_a_2:[0-9]+]] a: i64) -> i64 [linkage=external];
// IR-NEXT:     fn %[[VALUE_sysv_defined:[0-9]+]] @sysv_defined(%[[VALUE_a_3:[0-9]+]] a: i64, %[[VALUE_b_2:[0-9]+]] b: f64) -> i64 [linkage=external] [abi=sysv64 sysv_abi(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i64>(read<i64>(%[[VALUE_a_3]]), float_to_int<i64>(read<f64>(%[[VALUE_b_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_cb:[0-9]+]] cb: ptr<fn sysv_abi(i64, f64) -> i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_local:[0-9]+]] local: ptr<fn sysv_abi(i64, f64) -> i64> [storage=automatic] = function_decay<ptr<fn sysv_abi(i64, f64) -> i64>>(%[[VALUE_sysv_defined]]);
// IR-NEXT:         return add<i64>(add<i64>(add<i64>(add<i64>(call<i64, abi=sysv64 sysv_abi(scalar, scalar, scalar) -> scalar>(%[[VALUE_sysv_fixed]], const<i32>(1), const<f64>(2.0), widen<i64>(const<i32>(3))), call<i64, abi=sysv64 sysv_abi(scalar, scalar, scalar) -> scalar>(%[[VALUE_sysv_variadic]], const<i32>(2), const<i64>(1), const<f64>(2.5))), call<i64>(%[[VALUE_ms_native]], widen<i64>(const<i32>(1)))), call<i64, abi=sysv64 sysv_abi(scalar, scalar) -> scalar>(read<ptr<fn sysv_abi(i64, f64) -> i64>>(%[[VALUE_cb]]), widen<i64>(const<i32>(1)), const<f64>(2.0))), call<i64, abi=sysv64 sysv_abi(scalar, scalar) -> scalar>(read<ptr<fn sysv_abi(i64, f64) -> i64>>(%[[VALUE_local]]), widen<i64>(const<i32>(3)), const<f64>(4.0)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
