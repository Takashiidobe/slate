void abort(void);
void exit(int);

struct complex {
  float r;
  float i;
};

struct complex cmplx(float, float);

struct complex f(float a, float b) {
  struct complex c;
  c.r = a;
  c.i = b;
  return c;
}

int main(void) {
  struct complex z = f(1.0, 0.0);

  if (z.r != 1.0 || z.i != 0.0)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_complex:[0-9]+]] complex = struct {
// DEFAULT-NEXT:         field0 r: f32;
// DEFAULT-NEXT:         field1 i: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_cmplx:[0-9]+]] @cmplx(%[[VALUE1:[0-9]+]] <unnamed>: f32, %[[VALUE2:[0-9]+]] <unnamed>: f32) -> @type[[TYPE_complex]] [linkage=external] [abi=sysv64(scalar, scalar) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: f32, %[[VALUE_b:[0-9]+]] b: f32) -> @type[[TYPE_complex]] [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_complex]] [storage=automatic];
// DEFAULT-NEXT:         write<f32>(field0(%[[VALUE_c]]), read<f32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<f32>(field1(%[[VALUE_c]]), read<f32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_complex]], reason=return>(read<@type[[TYPE_complex]]>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE_complex]] [storage=automatic] = copy<@type[[TYPE_complex]], reason=assign>(call<@type[[TYPE_complex]], signature=fn(f32, f32) -> @type[[TYPE_complex]], abi=sysv64(scalar, scalar) -> native_c>(%[[VALUE_f]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)), float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(0.0))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(field0(%[[VALUE_z]]))), const<f64>(1.0)), ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(field1(%[[VALUE_z]]))), const<f64>(0.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
