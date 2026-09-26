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
// DEFAULT-NEXT:     type @type0 complex = struct {
// DEFAULT-NEXT:         field0 r: f32;
// DEFAULT-NEXT:         field1 i: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @cmplx(%11 <unnamed>: f32, %12 <unnamed>: f32) -> @type0 [linkage=external] [abi=sysv64(scalar, scalar) -> coerce<pair<f32>>];
// DEFAULT-NEXT:     fn %4 @f(%5 a: f32, %6 b: f32) -> @type0 [linkage=external] [abi=sysv64(scalar, scalar) -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 c: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(field0(%7), read<f32>(%5));
// DEFAULT-NEXT:         write<f32>(field1(%7), read<f32>(%6));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 z: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn(f32, f32) -> @type0, abi=sysv64(scalar, scalar) -> coerce<pair<f32>>>(%4, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field0(%9))), const<f64>(1.0)), ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field1(%9))), const<f64>(0.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
