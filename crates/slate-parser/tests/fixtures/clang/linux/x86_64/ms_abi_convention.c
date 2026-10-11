// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct pair {
  double a;
  double b;
};

typedef long(__attribute__((ms_abi)) *ms_callback)(long, double);

long __attribute__((ms_abi)) ms_fixed(int a, double b, long c, float d, long e);
long __attribute__((ms_abi)) ms_variadic(int n, ...);
long __attribute__((sysv_abi)) sysv_native(long a);
struct pair __attribute__((ms_abi)) ms_aggregate(struct pair p);

long __attribute__((ms_abi)) ms_defined(long a, double b) { return a + (long)b; }

long use(ms_callback cb, struct pair p) {
  ms_callback local = ms_defined;
  struct pair q = ms_aggregate(p);
  return ms_fixed(1, 2.0, 3, 4.0f, 5) + ms_variadic(2, 1L, 2.5) + sysv_native(1) +
         cb(1, 2.0) + local(3, 4.0) + (long)q.a;
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
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_ms_callback:[0-9]+]] ms_callback = ptr<fn ms_abi(i64, f64) -> i64>;
// IR-NEXT:     fn %[[VALUE_ms_fixed:[0-9]+]] @ms_fixed(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: i64, %[[VALUE_d:[0-9]+]] d: f32, %[[VALUE_e:[0-9]+]] e: i64) -> i64 [linkage=external] [abi=win64 ms_abi(scalar, scalar, scalar, scalar, scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_ms_variadic:[0-9]+]] @ms_variadic(%[[VALUE_n:[0-9]+]] n: i32, ...) -> i64 [linkage=external] [abi=win64 ms_abi(scalar) -> scalar];
// IR-NEXT:     fn %[[VALUE_sysv_native:[0-9]+]] @sysv_native(%[[VALUE_a_2:[0-9]+]] a: i64) -> i64 [linkage=external];
// IR-NEXT:     fn %[[VALUE_ms_aggregate:[0-9]+]] @ms_aggregate(%[[VALUE_p:[0-9]+]] p: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=win64 ms_abi(native_c) -> native_c];
// IR-NEXT:     fn %[[VALUE_ms_defined:[0-9]+]] @ms_defined(%[[VALUE_a_3:[0-9]+]] a: i64, %[[VALUE_b_2:[0-9]+]] b: f64) -> i64 [linkage=external] [abi=win64 ms_abi(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i64>(read<i64>(%[[VALUE_a_3]]), float_to_int<i64>(read<f64>(%[[VALUE_b_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_cb:[0-9]+]] cb: ptr<fn ms_abi(i64, f64) -> i64>, %[[VALUE_p_2:[0-9]+]] p: @type[[TYPE_pair]]) -> i64 [linkage=external] [abi=sysv64(scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_local:[0-9]+]] local: ptr<fn ms_abi(i64, f64) -> i64> [storage=automatic] = function_decay<ptr<fn ms_abi(i64, f64) -> i64>>(%[[VALUE_ms_defined]]);
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: @type[[TYPE_pair]] [storage=automatic] = copy<@type[[TYPE_pair]], reason=assign>(call<@type[[TYPE_pair]], abi=win64 ms_abi(native_c) -> native_c>(%[[VALUE_ms_aggregate]], copy<@type[[TYPE_pair]], reason=arg>(read<@type[[TYPE_pair]]>(%[[VALUE_p_2]]))));
// IR-NEXT:         return add<i64>(add<i64>(add<i64>(add<i64>(add<i64>(call<i64, abi=win64 ms_abi(scalar, scalar, scalar, scalar, scalar) -> scalar>(%[[VALUE_ms_fixed]], const<i32>(1), const<f64>(2.0), widen<i64>(const<i32>(3)), const<f32>(4.0), widen<i64>(const<i32>(5))), call<i64, abi=win64 ms_abi(scalar, scalar, scalar) -> scalar>(%[[VALUE_ms_variadic]], const<i32>(2), const<i64>(1), const<f64>(2.5))), call<i64>(%[[VALUE_sysv_native]], widen<i64>(const<i32>(1)))), call<i64, abi=win64 ms_abi(scalar, scalar) -> scalar>(read<ptr<fn ms_abi(i64, f64) -> i64>>(%[[VALUE_cb]]), widen<i64>(const<i32>(1)), const<f64>(2.0))), call<i64, abi=win64 ms_abi(scalar, scalar) -> scalar>(read<ptr<fn ms_abi(i64, f64) -> i64>>(%[[VALUE_local]]), widen<i64>(const<i32>(3)), const<f64>(4.0))), float_to_int<i64>(read<f64>(field0(%[[VALUE_q]]))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
