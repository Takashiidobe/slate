/* { dg-skip-if "signed zero not supported" { "vax-*-*" } } */
extern void abort(void);

double __attribute__((noinline)) not_fabs(double x) {
  return x >= 0.0 ? x : -x;
}

int main() {
  double x = -0.0;
  double y;

  y = not_fabs(x);

  if (!__builtin_signbit(y))
    abort();

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @not_fabs(%2 x: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ge<f64, exceptions=ignore>(read<f64>(%2), const<f64>(0.0)), read<f64>(%2), neg<f64>(read<f64>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 x: f64 [storage=automatic] = neg<f64>(const<f64>(0.0));
// DEFAULT-NEXT:         let %5 y: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%5, call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%4)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%4));
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=sign_bit>(read<f64>(%5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
