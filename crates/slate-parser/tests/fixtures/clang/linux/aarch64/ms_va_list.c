// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct wide {
  long long a, b, c;
};

struct tiny {
  char a, b, c;
};

long long __attribute__((ms_abi)) variadic(int n, ...) {
  __builtin_ms_va_list ap, copy;
  __builtin_ms_va_start(ap, n);
  __builtin_ms_va_copy(copy, ap);
  long long first = __builtin_va_arg(ap, long long);
  double d = __builtin_va_arg(ap, double);
  struct wide w = __builtin_va_arg(ap, struct wide);
  struct tiny t = __builtin_va_arg(ap, struct tiny);
  __builtin_ms_va_end(copy);
  __builtin_ms_va_end(ap);
  return first + (long long)d + w.c + t.c + (long long)sizeof(ap);
}

long long consume(__builtin_ms_va_list ap) {
  char *raw = ap;
  return __builtin_va_arg(ap, long long) + (raw != 0);
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
// IR-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type[[TYPE_tiny:[0-9]+]] tiny = struct {
// IR-NEXT:         field0 a: u8;
// IR-NEXT:         field1 b: u8;
// IR-NEXT:         field2 c: u8;
// IR-NEXT:     } [size=3, align=1, offsets=[0, 1, 2]];
// IR-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_n:[0-9]+]] n: i32, ...) -> i64 [linkage=external] [abi=win_arm64 ms_abi(scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: ptr<u8> [storage=automatic];
// IR-NEXT:         let %[[VALUE_copy:[0-9]+]] copy: ptr<u8> [storage=automatic];
// IR-NEXT:         va_start<win64>(%[[VALUE_ap]]);
// IR-NEXT:         va_copy<win64>(%[[VALUE_copy]], %[[VALUE_ap]]);
// IR-NEXT:         let %[[VALUE_first:[0-9]+]] first: i64 [storage=automatic] = va_arg<i64, win64>(%[[VALUE_ap]]);
// IR-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = va_arg<f64, win64>(%[[VALUE_ap]]);
// IR-NEXT:         let %[[VALUE_w:[0-9]+]] w: @type[[TYPE_wide]] [storage=automatic] = copy<@type[[TYPE_wide]], reason=assign>(va_arg<@type[[TYPE_wide]], win64>(%[[VALUE_ap]]));
// IR-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_tiny]] [storage=automatic] = copy<@type[[TYPE_tiny]], reason=assign>(va_arg<@type[[TYPE_tiny]], win64>(%[[VALUE_ap]]));
// IR-NEXT:         va_end<win64>(%[[VALUE_copy]]);
// IR-NEXT:         va_end<win64>(%[[VALUE_ap]]);
// IR-NEXT:         return add<i64>(add<i64>(add<i64>(add<i64>(read<i64>(%[[VALUE_first]]), float_to_int<i64>(read<f64>(%[[VALUE_d]]))), read<i64>(field2(%[[VALUE_w]]))), widen<i64>(reinterpret<i32>(widen<u32>(read<u8>(field2(%[[VALUE_t]])))))), reinterpret<i64>(const<u64>(8)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_consume:[0-9]+]] @consume(%[[VALUE_ap_2:[0-9]+]] ap: ptr<u8>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_raw:[0-9]+]] raw: ptr<u8> [storage=automatic] = read<ptr<u8>>(%[[VALUE_ap_2]]);
// IR-NEXT:         return add<i64>(va_arg<i64, win64>(%[[VALUE_ap_2]]), widen<i64>(from_bool<i32>(ne<ptr<u8>>(read<ptr<u8>>(%[[VALUE_raw]]), null<ptr<u8>>))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
