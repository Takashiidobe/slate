struct c {
  double a;
} __attribute((packed)) __attribute((aligned));

extern void abort(void);

double g_expect = 32.25;

void f(unsigned x, struct c y) {
  if (x != 0)
    abort();

  if (y.a != g_expect)
    abort();
}

struct c e = {64.25};

int main(void) {
  struct c d = {32.25};
  f(0, d);

  g_expect = 64.25;
  f(0, e);
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_g_expect:[0-9]+]] g_expect: f64 [storage=static] = const<f64>(32.25) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = const<f64>(64.25)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: u32, %[[VALUE_y:[0-9]+]] y: @type[[TYPE_c]]) -> void [linkage=external] [abi=sysv64(scalar, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(field0(%[[VALUE_y]])), read<f64>(%[[VALUE_g_expect]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_c]] [storage=automatic] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = const<f64>(32.25));
// DEFAULT-NEXT:         call<void, signature=fn(u32, @type[[TYPE_c]]) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), copy<@type[[TYPE_c]], reason=arg>(read<@type[[TYPE_c]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_g_expect]], const<f64>(64.25));
// DEFAULT-NEXT:         call<void, signature=fn(u32, @type[[TYPE_c]]) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), copy<@type[[TYPE_c]], reason=arg>(read<@type[[TYPE_c]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
